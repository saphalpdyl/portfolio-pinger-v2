# Portfolio Process Pinger

A C++ application that sends process status information to my website for the "Apps running on my machine" section.

![image](https://github.com/user-attachments/assets/ddd1fbf9-1229-4208-b37c-a3a5de22899e)

## How It Works

The application monitors specified processes on my local machine and sends heartbeat signals to my portfolio website every 2 seconds. The implementation follows these steps:

- Reads a list of processes to monitor from `~/.pinger.config.json` by default
- For each process, checks if it's currently running
- Only boolean status (running/not running) is tracked
- Creates a JSON payload containing process statuses
- Generates an HMAC hash of the process statuses using a shared private key
- Adds the hash as a header in the payload
- Sends HTTP POST requests to the Vercel endpoint
- Includes the HMAC signature in request payload
- Maintains a 2-second heartbeat interval between requests
