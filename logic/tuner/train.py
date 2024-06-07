import ctypes
import numpy as np
import json
import torch
import torch.nn as nn

BATCH_SIZE = 65536
FEATURE_SIZE = 828
INPUT_SIZE = 414
DATA_SIZE = 7153653
BATCH_NUM = DATA_SIZE // BATCH_SIZE
BATCH_SIZE_MUL_FEATURES_SIZE = BATCH_SIZE * FEATURE_SIZE

parser = ctypes.CDLL("./parser.so")
parser.parse_batch.restype = None
parser.parse_batch.argtypes = [
    np.ctypeslib.ndpointer(ctypes.c_float),
    np.ctypeslib.ndpointer(ctypes.c_float),
    np.ctypeslib.ndpointer(ctypes.c_float),
]

class Evaluation(nn.Module):
    def __init__(self):
        super().__init__()
        self.mg_values = nn.Linear(INPUT_SIZE, 1, bias=False)
        torch.nn.init.zeros_(self.mg_values.weight)
        self.eg_values = nn.Linear(INPUT_SIZE, 1, bias=False)
        torch.nn.init.zeros_(self.eg_values.weight)

    def forward(self, features, phases):
        white, black = torch.hsplit(features, 2)
        return torch.sigmoid(
            torch.lerp(
                self.eg_values(white) - self.eg_values(black),
                self.mg_values(white) - self.mg_values(black),
                phases
            )
        )

def batch_data_loader(): # whole dataset is too much for ram to hold
    for _ in range(BATCH_NUM):
        features = np.zeros((BATCH_SIZE, FEATURE_SIZE), dtype=ctypes.c_float)
        phases = np.zeros((BATCH_SIZE, 1), dtype=ctypes.c_float)
        y = np.zeros((BATCH_SIZE, 1), dtype=ctypes.c_float)

        parser.parse_batch(features, phases, y)

        yield torch.from_numpy(features), torch.from_numpy(phases), torch.from_numpy(y)

parser.open_book()

# features = np.zeros((BATCH_SIZE, FEATURE_SIZE), dtype=ctypes.c_float)
# phases = np.zeros((BATCH_SIZE, 1), dtype=ctypes.c_float)
# y = np.zeros((BATCH_SIZE, 1), dtype=ctypes.c_float)

epochs = 50

Evaluation_ = Evaluation()

optimizer = torch.optim.Adam(Evaluation_.parameters(), lr=0.01)
loss_fn = nn.MSELoss()
# print(BATCH_NUM)
iters = 0
for epoch in range(epochs):
    # if epoch == 10 or epoch == 20:
        # optimizer.param_groups[0]["lr"] /= 10
    epoch_loss = 0
    for features, phases, y in batch_data_loader():
        # print(iters)
        iters += 1
        optimizer.zero_grad()
        y_pred = Evaluation_.forward(features, phases)
        loss = loss_fn(y_pred, y)
        epoch_loss += loss.item()
        loss.backward()
        optimizer.step()
    parser.reread()

    print(f"epoch {epoch+1}/{epochs}; averaged squared loss: {epoch_loss / BATCH_NUM}")

with open("tuned_pst_values_done.json", "w") as fh:
    json.dump({
        name: param.detach().cpu().numpy().tolist()
        for name, param in Evaluation_.named_parameters()
    }, fh)

parser.close_book()