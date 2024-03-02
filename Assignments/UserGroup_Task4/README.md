# Task 4

### 1. Create a supplementary (Secondary) group called pgroup with group ID of 30000
``` bash
usermod -u 2005 fooUser      # Changes User ID (UID)
groupmod -g 3000 fooGroup    # Changes the Group ID (GID)
# ------------------------------------------------------------
sudo useradd -u 1500 jane   # Create new user with custom ID
groupadd -g 1010 mygroup    # Create new group with custom ID
```

### 2. Lock any user created account so he can't log in
``` bash
sudo passwd -l userToBeLocked    # Locks a specific user

sudo passwd -u userToBeUnlocked  # Unlocks a specific user
```
> ℹ️ **Note: you can lock the currently used user ... which will take effect when you restart ... BECAREFUL to not lock all admin users, as you won't be able to switch to root unless you know root's password either by using _su_ or booting.**

#### 3. Delete user account
#### 4. Delete group account
``` bash
sudo userdel userToBeDeleted
sudo groupdel -f groupToBeDeleted
```
##### Options of userdel

|   Option  	| Description 	|
|:------:	|:------:	|
|  **-r**  	|   **Delete the user's home directory and files**  	|
| **-f** 	|  **Force the deletion of the user's account, even if processes are still running**  	|

### 5. State the difference between adduser and useradd with example shown.
###### **useradd**:
* **_useradd_** is a low-level command-line utility that adds a new user to the system.
* It requires specific options to be set to define the user's properties, such as home directory, shell, user ID, etc.
* It does not create a home directory by default unless explicitly specified with the -m option.
* It does not provide a user-friendly interface for setting user properties interactively; instead, it's typically used in scripts or automated processes.
  ``` bash
  # It's always recommended to use -m with useradd to create home directory ... additionally, /etc/skel/ contents needed to be moved (.bashrc, .profile, ...)
  sudo useradd -m username
  ```
###### **adduser**:
* **_adduser_** is a higher-level command that provides a more user-friendly and interactive interface for adding users.
* It prompts the administrator with a series of questions to set up the user account, such as full name, password, and other details.
* It automatically creates a home directory for the user.
* It manages some configuration files and settings automatically, making the process easier for administrators.

###### **deluser**:
* **_deluser_** is a higher-level command that provides a user-friendly interface for deleting user accounts.
* It takes care of removing the user's home directory and mailbox by default, but these actions can be controlled with options.
* It may prompt the administrator with additional options or warnings before deleting the user account.
* It may perform additional cleanup tasks, such as removing the user from any groups they belong to.

###### **userdel**:
* **_userdel_** is a low-level command-line utility for deleting user accounts.
* It requires specifying the username of the account to be deleted.
* By default, userdel does not remove the user's home directory or mailbox. If these need to be deleted, additional options need to be specified.
* It does not provide a user-friendly interface or prompts; it's typically used in scripts or automated processes.
