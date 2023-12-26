import os
import numpy as np


# Write data to file
def writes(data, datapath):
    # check that data only contains integers
    if not np.issubdtype(data.dtype, np.integer):
        raise ValueError('data must be integer type')

    if not np.issubdtype(data.dtype, np.int16):
        data = data.astype(np.int16)

    # check that data is 1D
    if data.ndim != 1:
        raise ValueError('data must be 1D')

    # write data to file in Little-endian ordering
    with open(datapath, 'wb') as f:
        data.tofile(f)


# Read data from file
def reads(datapath):
    # check that file exists
    if not os.path.isfile(datapath):
        raise ValueError('file does not exist')

    # read data from file in Little-endian ordering
    with open(datapath, 'rb') as f:
        data = np.fromfile(f, dtype=np.int16)
    return data
