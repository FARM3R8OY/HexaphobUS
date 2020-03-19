## Setup
- [Installations](#Install)
	- [Install KMS](#insKMS)
	- [Install Git](#insGit)
	- [Install IDE](#insIDE)
	- [Upgrade Python on Debian (Raspberry Pi)](#PyUpgrade)
- [Key Management](#KeyManage)
	- [Generate keys](#kmGen)
	- [Export keys](#kmExp)
- [Git Configuration](#GitConfig)
	- [Stage, commit and push using Git](#DirectGit)
- [IDE Configuration](#IDEConfig)
	- [Git on VSCode / VSCodium](#IDEGitConfig)
	- [Arduino on VSCode / VSCodium](#IDEArduinoConfig)

[//]: # (------------------------------------------------)

This setup procedure is provided for people who wish to know the details about key management for signed commits, and integration of Git and Arduino in an IDE of their choice. Personally, we chose VSCodium for Windows 10 and Debian 9 (Raspberry Pi 3+) as our IDE, with the Arduino IDE as a dependency, and GnuPG as our key management system.

Notes:
- Git integration might be easier with GitHub Desktop or GitKraken. To be explored.
- Key management can also work with an SSH protocol. To be explored.
- If you wish to add your own procedure to this file, contact one of the contributors.

[//]: # (------------------------------------------------)

## <a id="Install"></a>Installations

#### <a id="insKMS"></a>Install Key Management System (KMS)

- Linux: [GnuPG](https://www.gnupg.org/download/)
- Mac: [GnuPG for OS X](https://sourceforge.net/p/gpgosx/docu/Download/) and [GPG tools](https://gpgtools.org/)
- Windows: [Gpg4win](https://gpg4win.org/download.html)

#### <a id="insGit"></a>Install Git

- [Linux](https://git-scm.com/download/linux)
- [Mac](https://git-scm.com/download/mac)
- [Windows](https://git-scm.com/download/win)

#### <a id="insIDE"></a>Install Integrated Development Environment (IDE)

- [Arduino](https://www.arduino.cc/en/main/software)
- [VSCode](https://code.visualstudio.com/)
- [VSCodium](https://github.com/VSCodium/vscodium/releases)

#### <a id="PyUpgrade"></a>Upgrade Python on Debian (Raspberry Pi)

If you wish to run Python on the latest and greatest on your Raspberry Pi, you must install from the source code. The sequence of command lines is as follows:
- Install prerequisites:
	```bash
	sudo apt-get update
	sudo apt-get install -y build-essential tk-dev libncurses5-dev libncursesw5-dev |
	libreadline6-dev libdb5.3-dev libgdbm-dev libsqlite3-dev libssl-dev libbz2-dev |
	libexpat1-dev liblzma-dev zlib1g-dev libffi-dev openssl tar wget vim
	```
- Download Python:
	```bash
	cd /opt
	wget https://www.python.org/ftp/python/3.8.0/Python-3.8.2.tgz
	```
- Extract: 
	```bash
	sudo tar zxf Python-3.8.2.tgz
	cd Python-3.8.2
	sudo ./configure --enable-optimizations
	sudo make -j 4 # Number of cores on Pi (can be found with command 'nproc')
	sudo make altinstall
	```
- Make it your default version:
	```bash
	python3.8 -V
	echo "alias python=/usr/local/bin/python3.8" >> ~/.bashrc
	source ~/.bashrc
	python -V
	```
- Install modules:
	```bash
	sudo apt-get install python3-pyqt5
	```

Don't forget to delete the archive and the extracted folder after successful installation.

[//]: # (------------------------------------------------)

## <a id="KeyManage"></a>Key Management

#### <a id="kmGen"></a>Generate keys

Open Git Bash or Command Prompt, and generate a set of public and private keys.
```bash
gpg --full-generate-key

# For GnuPG version 2.1.16 and below:
gpg --default-new-key-algo rsa4096 --gen-key
```

Following [Github](https://help.github.com/en/github/authenticating-to-github/generating-a-new-gpg-key)'s recommendations:
- Select the default protocol (RSA and RSA);
- Select a 4096 bits key;
- Select an expiration date (none for minimum maintenance);
- Confirm;
- Enter your name (real name, user name, etc.);
- Enter your verified GitHub email adress (**important**);
- Enter a comment (optional);
- Enter a passphrase (this will allow you to access restricted GPG operations and sign your commits).

#### <a id="kmExp"></a>Export keys

Still in Git Bash or Command Prompt, display your newly-created keys.
```bash
gpg --list-secret-keys --keyid-format LONG
```

The resulting text will look something like this:
```bash
	sec   rsa4096/<sec-key-ID> <creation-date> [SC]
	      <long-sec-key-ID>
	uid                 [ultimate] <username> <email@example.com>
	ssb   rsa4096/<ssb-key-ID> <creation-date> [E]
```

Select the public key ID (in the example above, ```<sec-key-ID>```) and run the GPG export command.
```bash
gpg --armor --export <sec-key-ID>
```

The entire public key should now be displayed:
```bash
-----BEGIN PGP PUBLIC KEY BLOCK-----
*Huge chunk of characters*
-----END PGP PUBLIC KEY BLOCK-----
```

Copy the entire output, and setup a new GPG key on [GitHub](https://github.com/settings/keys).

Now we only need to register the public key to the GnuPG server, in order for the signed commits to work properly.
- Run ```gpg --gen-revoke <sec-key-ID>```;
- Confirm the revocation certificate;
- Run ```gpg --send-keys <sec-key-ID>``` (**this only sends the public key**).

The key management procedure is now complete. You can close the window.

[//]: # (------------------------------------------------)

## <a id="GitConfig"></a>Git Configuration

The Git configuration can be managed at a local or a global level, depending on your needs. For our purposes, we will use a global config.

Open the Windows Command Prompt and run the following commands. If any issue is encountered later on, try again with an elevated Command Prompt:
- User-related:
	- ```git config --global user.name <username>``` (same username as your GitHub profile);
	- ```git config --global user.email <email>``` (same email as your GitHub verified email address);
	- ```git config --global user.signingkey <sec-key-ID>```.
- GPG-related:
	- If key management was done on Git Bash: ```git config --global gpg.program "<drive>:\<programs>\Git\usr\bin\gpg.exe"```;
	- On Command Prompt: ```git config --global gpg.program "<drive>:\<programs>\GnuPG\bin\gpg.exe"```.
- Version control-related:
	- ```git config --global commit.gpgsign true``` (automatically signs commits).
	
The Git configuration procedure is now complete. You can close the window.

#### <a id="DirectGit"></a>Stage, commit, and push using Git

To change the repository with files that aren't compatible with your IDE (CAD files, for instance), simply follow this procedure in a Git Bash window opened in your repository clone directory:

```bash
# Stage changes
git add .

# Check new and modified files for approval
git status

# Commit with message
git commit -a -m <msg> # or simply 'git commit -a', to open a multiline commit message in your default text editor

# Push in the right branch
git push origin <branch>
```

For more option or a deeper understanding of Git and its commands, refer to the [Git documentation](https://git-scm.com/docs).

[//]: # (------------------------------------------------)

## <a id="IDEConfig"></a>IDE Configuration

#### <a id="IDEGitConfig"></a>Git on VSCode / VSCodium

To configure Git in VSCode / VSCodium, open one IDE and follow these steps:
- Extensions (<kbd>Ctrl</kbd>+<kbd>Shift</kbd>+<kbd>X</kbd>): Make sure that the "Git Extension Pack" extension is installed (and any other Git extension you find useful);
- File &#8594; Preferences &#8594; Settings (<kbd>Ctrl</kbd>+<kbd>,</kbd>) &#8594; Extensions &#8594; Git &#8594; Enable Commit Signing;
	- Alternatively, Show All Commands (<kbd>Ctrl</kbd>+<kbd>Shift</kbd>+<kbd>P</kbd>) &#8594; Preferences: Open settings (JSON) &#8594; Add: ```"git.enableCommitSigning": true```. You can also add the command to the workspace JSON settings, it will override the user defaults. If you set it in the workspace, make sure every collaborator has setup their KMS, else the command will prevent them from committing to the repository.

#### <a id="IDEArduinoConfig"></a>Arduino on VSCode / VSCodium	

To configure Arduino in VSCode / VSCodium, follow these steps:
- Make sure that you install the [Arduino IDE](#insIDE): the Arduino extension on VSCode / VSCodium will need its executables;
- Find your Arduino path file ("<drive:\<programs>\Arduino", for instance) and add the [Sunfounder PWM Servo Driver library](../README.md#PWM) to the Arduino libraries ("<drive>:\<programs>\Arduino\libraries" with folder name "Adafruit_PWMServoDriver", for instance). This step is specific to our project;
- On VSCode or VSCodium;
	- Extensions (<kbd>Ctrl</kbd>+<kbd>Shift</kbd>+<kbd>X</kbd>): Make sure that the "Arduino" extension is installed (and any other Arduino/C/C++ extension you find useful);
	- Show All Commands (<kbd>Ctrl</kbd>+<kbd>Shift</kbd>+<kbd>P</kbd>) &#8594; Preferences: Open settings (JSON) &#8594; Add the following parameters (according to the [VSCode Arduino extension's GitHub page](https://github.com/microsoft/vscode-arduino)): 
		```bash
		"arduino.path": "<drive>:\\<programs>\\Arduino",
		"arduino.commandPath": "arduino_debug.exe",
		"arduino.logLevel": "info", 
		"arduino.enableUSBDetection": true,
		"arduino.disableTestingOpen": false,
		"arduino.skipHeaderProvider": false,
		"arduino.additionalUrls": [
		"https://raw.githubusercontent.com/VSChina/azureiotdevkit_tools/master/package_azureboard_index.json",
		"http://arduino.esp8266.com/stable/package_esp8266com_index.json"
		],
		"arduino.defaultBaudRate": 115200
		```

	- Inside your workspace's ".vscode" folder, add the file "arduino.json" and write the following parameters:
		```bash
		{
		    "port": "<port_name>",
		    "board": "arduino:avr:<type>",
		    "configuration": "cpu=<cpu_type>",
		    "sketch": "<file_directory>\\<file_name>.ino"
		}
		```
	- Still in the same folder, add the file "c_cpp_properties.json" and write the following parameters:
		```bash
		{
		    "configurations": [
			{
			    "name": "Win32",
			    "includePath": [
				"${workspaceFolder}\\**",
				"<drive>:\\<programs>\\Arduino\\tools\\**",
				"<drive>:\\<programs>\\Arduino\\libraries\\**",
				"<drive>:\\<programs>\\Arduino\\hardware\\arduino\\avr\\**",
				"<drive>:\\<programs>\\Arduion\\hardware\\arduino\\avr\\cores\\arduino",
				"<drive>:\\<programs>\\Arduino\\hardware\\arduino\\avr\\libraries\\EEPROM\\src",
				"<drive>:\\<programs>\\Arduino\\hardware\\arduino\\avr\\libraries\\HID\\src",
				"<drive>:\\<programs>\\Arduino\\hardware\\arduino\\avr\\libraries\\SoftwareSerial\\src",
				"<drive>:\\<programs>\\Arduino\\hardware\\arduino\\avr\\libraries\\SPI\\src",
				"<drive>:\\<programs>\\Arduino\\hardware\\arduino\\avr\\libraries\\Wire\\src",
				"<drive>:\\<programs>\\Arduino\\hardware\\arduino\\avr\\libraries\\SPI\\src"
			    ],
			    "defines": [
				"_DEBUG",
				"UNICODE",
				"_UNICODE",
				"F_CPU=16000000L",
				"ARDUINO=10805",
				"ARDUINO_AVR_UNO",
				"ARDUINO_ARCH_AVR"
			    ],
			    "compilerPath": "<drive>:\\<programs>\\Arduino\\hardware\\tools\\avr\\bin\\avr-gcc.exe",
			    "cStandard": "c11",
			    "cppStandard": "c++17",
			    "intelliSenseMode": "msvc-x64",
			    "forcedInclude": [
				"<drive>:\\<programs>\\Arduino\\hardware\\arduino\\avr\\cores\\arduino\\Arduino.h"
			    ]
			}
		    ],
		    "version": 4
		}
		```
	
The IDE configuration procedure is now complete. You can close the window.

[//]: # (------------------------------------------------)

##
That's it! You should have integrated Git and Arduino into your favorite IDE now, and allowed a KMS to sign your commits. You have also upgraded the Python version on your Raspberry Pi. If any issue persists, you can find help online on Stack Overflow, the GitHub forums, and more.
