# Portfolio Process Pinger

A C++ application that sends process status information to my website for the "Apps running on my machine" section.

![image](https://github.com/user-attachments/assets/d247e23b-5797-46e8-bcb2-4a30635c0267)

## Installation
Build the project with CMake.
```bash
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=<path_to_vcpkg_installation>/vcpkg/scripts/buildsystems/vcpkg.cmake
```

Then, follow up with these commands:
- `install`: Registers the systemd service
- `uninstall`: Completely uninstalls the service from the machine
- `run`: Internally used by systemd to run the service

```bash
sudo ./portfolio_pinger install
```

## Configuration
By default, the program expects the configuration to live in `/etc/${SERVICE_NAME}/pinger.config.json`. Create the folder using `mkdir /etc/${SERVICE_NAME}/`. Create the `pinger.config.json` in home and generate a symlink to `/etc/${SERVICE_NAME}/pinger.config.json` to easily change configuration from the home directory. For the default SERVICE_NAME `portfolio-process-monitor`:

```bash
mkdir /etc/portfolio-process-monitor/
cd ~ && touch pinger.config.json
ln -s ~/pinger.config.json /etc/portfolio-process-monitor/pinger.config.json 
```

## Changing service name or configuration directory
You can change the service name or configuration directory during build in `CMakeLists.txt` file.
```cmake
add_definitions(-DENV_SERVICE_NAME="your-service-name")
add_definitions(-DCONFIGURATION_HOME_DIRECTORY="/path/to/config")
```

## Example configuration
```json
{
  "authorization_secret": "secret123",
  "target_url": "<your-url-endpoint>",
  "ping_interval": 1000,
  "process_targets": [
    {
      "app_name": "Chrome",
      "process_name": "chrome"
    },
    {
      "app_name": "Android Studio",
      "process_name": "studio"
    },
    {
      "app_name": "Postman",
      "process_name": "postman"
    },
    {
      "app_name": "Visual Studio IDE",
      "process_name": "visual_studio"
    },
    {
      "app_name": "Visual Studio Code",
      "process_name": "code"
    }
  ]
}
```

## How It Works: Deprecated

![image](https://github.com/user-attachments/assets/ddd1fbf9-1229-4208-b37c-a3a5de22899e)

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

[![Tech Stack](https://skillicons.dev/icons?i=cpp,cmake)]()
