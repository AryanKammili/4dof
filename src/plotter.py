import serial
import serial.tools.list_ports
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from collections import deque
import sys

BAUD_RATE = 115200
WINDOW_SIZE = 500
SERIAL_PORT = None

# Expected serial line format (CSV, printed once per loop() by main.cpp):
#   relative_deg, absolute_deg, raw_angle_deg, agc, magnitude


def find_serial_port():
    ports = list(serial.tools.list_ports.comports())

    if len(ports) == 0:
        print("No Serial Ports, ESP32 not detected")
        sys.exit(1)

    for port in ports:
        if "usbmodem" in port.device or "usbserial" in port.device:
            print(f"Auto Detected port: {port.device}")
            return port.device

    print("Available Ports")

    for index, port in enumerate(ports):
        print(f" [{index}] {port.device} - {port.description}")
    index = int(input("Selected port index: "))
    return ports[index].device


def main():
    port = SERIAL_PORT or find_serial_port()
    ser = serial.Serial(port=port, baudrate=BAUD_RATE, timeout=1)
    print(f"Connected to {port} at {BAUD_RATE} baud rate")

    x_data = deque(maxlen=WINDOW_SIZE)
    relative_data = deque(maxlen=WINDOW_SIZE)
    absolute_data = deque(maxlen=WINDOW_SIZE)
    raw_angle_data = deque(maxlen=WINDOW_SIZE)
    agc_data = deque(maxlen=WINDOW_SIZE)
    magnitude_data = deque(maxlen=WINDOW_SIZE)
    sample_count = 0

    fig, (ax_pos, ax_diag) = plt.subplots(2, 1, figsize=(10, 8), sharex=True)

    # --- Top panel: position signals (degrees) ---
    line_rel, = ax_pos.plot([], [], label="Relative Position (deg)", linewidth=1.5)
    line_abs, = ax_pos.plot([], [], label="Absolute Position (deg)", linewidth=1.5, alpha=0.7)
    line_raw, = ax_pos.plot([], [], label="Raw Angle (deg, unprocessed)", linewidth=1.2, alpha=0.6, linestyle="--")
    ax_pos.set_ylabel("Degrees")
    ax_pos.set_title("AS5600 Encoder Position (live)")
    ax_pos.legend(loc="upper left")
    ax_pos.grid(True, alpha=0.3)

    # --- Bottom panel: diagnostics (AGC + magnitude on separate axes, different scales) ---
    ax_mag = ax_diag.twinx()
    line_agc, = ax_diag.plot([], [], label="AGC (0-255)", color="tab:orange", linewidth=1.5)
    line_mag, = ax_mag.plot([], [], label="Magnitude (0-4095)", color="tab:green", linewidth=1.5, alpha=0.7)

    ax_diag.set_xlabel("Sample #")
    ax_diag.set_ylabel("AGC", color="tab:orange")
    ax_diag.tick_params(axis="y", labelcolor="tab:orange")
    ax_diag.set_ylim(0, 255)
    ax_diag.grid(True, alpha=0.3)

    ax_mag.set_ylabel("Magnitude", color="tab:green")
    ax_mag.tick_params(axis="y", labelcolor="tab:green")
    ax_mag.set_ylim(0, 4095)

    # Combined legend for the diagnostics panel
    lines_diag = [line_agc, line_mag]
    ax_diag.legend(lines_diag, [l.get_label() for l in lines_diag], loc="upper left")

    def update(frame):
        nonlocal sample_count
        # Drain everything waiting in the buffer this frame
        while ser.in_waiting:
            try:
                raw_line = ser.readline().decode("utf-8", errors="ignore").strip()
            except UnicodeDecodeError:
                continue

            if not raw_line:
                continue

            parts = raw_line.split(",")
            if len(parts) < 5:
                continue

            try:
                rel = float(parts[0])
                abs_ = float(parts[1])
                raw_ang = float(parts[2])
                agc = float(parts[3])
                mag = float(parts[4])
            except ValueError:
                continue

            sample_count += 1
            x_data.append(sample_count)
            relative_data.append(rel)
            absolute_data.append(abs_)
            raw_angle_data.append(raw_ang)
            agc_data.append(agc)
            magnitude_data.append(mag)

        if x_data:
            line_rel.set_data(x_data, relative_data)
            line_abs.set_data(x_data, absolute_data)
            line_raw.set_data(x_data, raw_angle_data)
            line_agc.set_data(x_data, agc_data)
            line_mag.set_data(x_data, magnitude_data)

            ax_pos.set_xlim(max(0, x_data[0]), x_data[-1] + 1)

            pos_vals = list(relative_data) + list(absolute_data) + list(raw_angle_data)
            ymin, ymax = min(pos_vals), max(pos_vals)
            pad = max(5, (ymax - ymin) * 0.1)
            ax_pos.set_ylim(ymin - pad, ymax + pad)

        return line_rel, line_abs, line_raw, line_agc, line_mag

    ani = FuncAnimation(fig, update, interval=30, cache_frame_data=False)
    plt.tight_layout()
    plt.show()

    ser.close()


if __name__ == "__main__":
    main()