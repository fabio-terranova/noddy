import time
from contextlib import contextmanager

import matplotlib.pyplot as plt
import noddy_py
import numpy as np
from scipy import signal

plot = True
n_samples = 60000
n_channels = 64
order = 5
fc = 1000
fs = 10000
btype = "low"
ftype = "butter"
spike_frequency = 50  # Hz


def spike(samples: int, fs: float, amplitude: float = 1.0) -> np.ndarray:
    # the spike lasts about 1 ms
    t = np.arange(-samples // 2, samples // 2) / fs
    spike_waveform = (
        amplitude * (1 - (t * fs / 10) ** 2) * np.exp(-((t * fs / 10) ** 2))
    )
    return spike_waveform


@contextmanager
def timed(label: str):
    t0 = time.time()
    try:
        yield
    finally:
        dt = time.time() - t0
        print(f"{label}:\t {dt:.4f} s")


b, a = signal.iirfilter(order, fc, fs=fs, btype=btype, ftype=ftype, rs=5.0, rp=5.0)

# create multichannel neural spike-like data
np.random.seed(42)
t = np.arange(n_samples) / fs
data = np.random.randn(n_channels, n_samples)
n_spikes = int(spike_frequency * (n_samples / fs))
spike_times = np.random.choice(n_samples - 20, n_spikes, replace=False) + 10
for ch in range(n_channels):
    delay = np.random.randint(-10, 11)  # delay between -10 and 10 samples
    for st in spike_times:
        if 10 <= st + delay < n_samples - 10:
            # the longer the delay the smaller the amplitude
            amplitude = 5.0 - abs(delay) / 2.0
            data[ch, st + delay - 10 : st + delay + 10] += spike(20, fs, amplitude)

with timed("Time taken (py, axis=1)"):
    output_py = signal.lfilter(b, a, data, axis=1)

with timed("Time taken (cpp, multi)"):
    output_cpp_multi = np.array(noddy_py.lfilter_multi(b=b, a=a, x=data))

if plot:
    # downsample for plotting
    ds_factor = 10
    t = t[::ds_factor]
    data = data[:, ::ds_factor]
    output_py = output_py[:, ::ds_factor]
    output_cpp_multi = output_cpp_multi[:, ::ds_factor]
    # square grid of subplots
    n = int(np.ceil(np.sqrt(n_channels)))
    fig, axes = plt.subplots(n, n, figsize=(12, 8), sharex=True, sharey=True)
    axes = axes.flatten()
    for ch in range(n_channels):
        axes[ch].plot(t, data[ch, :], label="input", alpha=0.3)
        axes[ch].plot(t, output_py[ch, :], label="scipy", alpha=0.7)
        axes[ch].plot(
            t, output_cpp_multi[ch, :], label="noddy multi", linestyle="--", alpha=0.7
        )
    axes[0].legend()
    plt.tight_layout()
    plt.show()
