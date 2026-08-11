import serial
import serial.tools.list_ports
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from collections import deque
import sys

BAUD_RATE = 115200
WINDOW_SIZE = 500
SERIAL_PORT = None

def find_serial_port():
    ports = list(serial.tools.list_ports.comports())

    if len(ports) == 0:
        print("No Serial Ports, ESP32 not detected")
        sys.exit(1)

    for port in ports:
        if "usbmodem" in port.device:
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

    relative_data = deque(maxlen=WINDOW_SIZE)
    absolute_data = deque(maxlen=WINDOW_SIZE)
    x_data = deque(maxlen=WINDOW_SIZE)
    sample_count = 0

    fig, ax = plt.subplots(figsize=(10, 6))
    line_rel, = ax.plot([], [], label="Relative Position (deg)", linewidth=1.5)
    line_abs, = ax.plot([], [], label="Absolute Position (deg)", linewidth=1.5, alpha=0.6)
    ax.set_xlabel("Sample #")
    ax.set_ylabel("Degrees")
    ax.set_title("AS5600 Encoder Position (live)")
    ax.legend(loc="upper left")
    ax.grid(True, alpha=0.3)

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
            if len(parts) < 2:
                continue

            try:
                rel = float(parts[0])
                abs_ = float(parts[1])
            except ValueError:
                continue

            sample_count += 1
            x_data.append(sample_count)
            relative_data.append(rel)
            absolute_data.append(abs_)

        if x_data:
            line_rel.set_data(x_data, relative_data)
            line_abs.set_data(x_data, absolute_data)
            ax.set_xlim(max(0, x_data[0]), x_data[-1] + 1)

            all_vals = list(relative_data) + list(absolute_data)
            ymin, ymax = min(all_vals), max(all_vals)
            pad = max(5, (ymax - ymin) * 0.1)
            ax.set_ylim(ymin - pad, ymax + pad)

        return line_rel, line_abs

    ani = FuncAnimation(fig, update, interval=30, cache_frame_data=False)
    plt.tight_layout()
    plt.show()

    ser.close()


if __name__ == "__main__":
    main()