## Setup
- [Installations](#Install)
	- [Install KMS](#insKMS)
	- [Install Git](#insGit)
	- [Install IDE](#insIDE)
- [Key Management](#KeyManage)
	- [Generate keys](#kmGen)
	- [Export keys](#kmExp)
- [Git Configuration](#GitConfig)
- [IDE Configuration](#IDEConfig)

[//]: # (------------------------------------------------)

This setup procedure is provided for people who wish to know the details about key management for signed commits and integration with Git and an IDE of their choice. Personally, we chose VSCodium for Windows 10 and Debian 9 (Raspberry Pi 3+) as our IDE, and GnuPG as our key management system.

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

- [VSCode](https://code.visualstudio.com/)
- [VSCodium](https://github.com/VSCodium/vscodium/releases)

[//]: # (------------------------------------------------)

## <a id="KeyManage"></a>Key Management

#### <a id="kmGen"></a>Generate keys

Open Git Bash, and generate a set of public and private keys.
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

Still in Git Bash, display your newly-created keys.
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
- Confirm the revocation certificate.
- Run ```gpg --send-keys <sec-key-ID>``` (**this only sends the public key**).

The key management procedure is now complete. You can exit Git Bash.

[//]: # (------------------------------------------------)

## <a id="GitConfig"></a>Git Configuration

[//]: # (------------------------------------------------)

## <a id="IDEConfig"></a>IDE Configuration

5. Open Command Prompt 
	5.0. If any issue is encountered, open an elevated Command Prompt.
	5.1. git config --global user.name <username> (same username as GitHub)
	5.2. git config --global user.name <email> (same email as GitHub and GPG)
	5.3. git config --global user.signingkey <sec-key-ID>
	5.4. git config --global gpg.program "<drive>:\<programs>\Git\usr\bin\gpg.exe"
		5.4.0. If signing failed later, try: git config --global gpg.program "<drive>:\<programs>\GnuPG\bin\gpg.exe"
	5.5. git config --global commit.gpgsign true (automatically signs commits)
	5.6. exit
6. Open VSCodium
	6.0. Make sure that "Git Extension Pack" is installed
	6.1. Show All Commands -> Preferences: Open settings (JSON) -> Add: "git.enableCommitSigning": true
	6.2. Alternatively: Settings -> Extensions -> Git -> Enable Commit Signing
