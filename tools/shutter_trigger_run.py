import subprocess
import time
import argparse

cmd = r"C:\Users\User\Desktop\xi410_direct_sdk_project\xi410_tcp_trigger\build\bin\Debug\xi410_tcp_trigger.exe  -e C:\Users\User\Desktop\xi410_direct_sdk_project\tools\test -n example.csv -p"

def run_cmd(n, delay):
    for i in range(n):
        print(f"Running CMD instance {i+1}/{n}...")
        subprocess.run(cmd, shell=True)  # Execute cmd.exe
        if i < n - 1:  # Avoid sleeping after the last execution
            time.sleep(delay)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Execute cmd.exe N times with delay")
    parser.add_argument("-n", type=int, required=True, help="Number of times to execute cmd.exe")
    parser.add_argument("-d", "--delay", type=float, required=True, help="Delay (in seconds) between executions")

    args = parser.parse_args()

    run_cmd(args.n, args.delay)