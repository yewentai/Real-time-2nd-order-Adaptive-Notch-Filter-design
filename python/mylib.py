import numpy as np
from scipy.signal import welch


def mean_log_spectral_distortion(original_signal, filtered_signal, sample_rate):
    """
    Calculates the Mean Log-Spectral Signal Distortion (SD).

    :param original_signal: The original signal (1D numpy array).
    :param filtered_signal: The filtered signal (1D numpy array).
    :param sample_rate: Sample rate of the signals (in Hz).
    :return: Mean Log-Spectral Signal Distortion.
    """
    # Compute the power spectral density of the original and filtered signals
    f, Pxx = welch(original_signal, fs=sample_rate)
    _, Pyy = welch(filtered_signal, fs=sample_rate)

    # Avoid division by zero
    Pxx[Pxx == 0] = 1e-12
    Pyy[Pyy == 0] = 1e-12

    # Calculate the log-spectral distortion
    distortion = np.sqrt(np.trapz((10 * np.log10(Pyy / Pxx)) ** 2, f))

    return distortion


# Example usage
# sd = mean_log_spectral_distortion(original_signal, filtered_signal, sample_rate)
# print("Mean Log-Spectral Signal Distortion:", sd)
