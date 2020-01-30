1. Install VSCodium (https://github.com/VSCodium/vscodium)
2. Install GNUPG (https://www.gnupg.org/download/)
3. Install Git (https://git-scm.com/downloads)
4. Open Git Bash
	4.1. gpg --full-generate-key
		4.1.0. Version 2.1.16 and below: gpg --default-new-key-algo rsa4096 --gen-key (skip to 3.1.3)
		4.1.1. Select 1 (RSA and RSA) (default)
		4.1.2. Select 4096 bits
		4.1.3. Select 0 (no expiration date)
		4.1.4. Confirm
		4.1.5. Enter name (real name, user name, etc.)
		4.1.6. Enter GitHub email 
		4.1.7. Enter comment (optional)
		4.1.8. Enter passphrase (used to access restricted GPG operations)
	4.2. gpg --list-secret-keys --keyid-format LONG
		4.2.0. Example:
			sec   rsa4096/<sec-key-ID> 2020-01-30 [SC]
			      <long-sec-key-ID>
			uid                 [ultimate] username <email@example.com>
			ssb   rsa4096/<ssb-key-ID> 2020-01-30 [E]
		4.2.1. Select the key ID (in the example above, <sec-key-ID>)
	4.3. gpg --armor --export <sec-key-ID>
		4.2.0. Output:
			-----BEGIN PGP PUBLIC KEY BLOCK-----

			*Huge chunk of characters*
			-----END PGP PUBLIC KEY BLOCK-----
		4.2.1. Copy the output
		4.2.2. Add a new GPG key on GitHub with the output (https://github.com/settings/keys)
	4.4. gpg --gen-revoke <sec-key-ID>
		4.4.1. Confirm
	4.5. gpg --send-keys <sec-key-ID>
		4.5.0. This only sends the public key, not the private key.
	4.6. exit
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
