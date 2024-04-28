# Task 5

### 1. Create a folder called myteam in your home directory and change its permissions to read only for the owner.

```bash
# $_ --> is a special variable that holds the last argument from the previous command executed in the shell.
mkdir myteam && sudo chmod 400 $_
```

---

### 2. Log out and log in by another user

```bash
sudo pkill -KILL -u enigma
```

3. Try to access (by cd command) the folder (myteam)

   > Not accessable for anyone _(except root)_ ... because cd needs execute permissions
   > The read ( r ) permission lets users look **( ls )** into directories. The execute ( x ) permission lets users move **( cd )** into directories.

4. Using the command Line
   a. Change the permissions of oldpasswd file to give owner read and write permissions and for group write and execute and execute only for the others (using chmod in 2 different ways)

```bash
# Using Octal representation
sudo chmod 631 oldpasswd
# OR
# Using symbolic/word representation
sudo chmod u+rw oldpasswd && sudo chmod g+wx oldpasswd && sudo chmod o+x oldpasswd   # First Method (Not Efficient)
sudo chmod u=rw,g=wx,o=x oldpasswd      # Make sure there are no spaces !!!
```

b. Change your default permissions to be as above.

```bash
umask 036
```

c. What is the maximum permission a file can have, by default when it is just created? And what is that for directory.

- **New files:** 666 - 022 = 644
- **New directories:** 777 - 022 = 755

d. Change your default permissions to be no permission to everyone then create a directory and a file to verify.

```bash
umask 077
```

e. State who can access this file if any with proof.

> In this case, only the user (owner) has read and write permission, while the group and others have no permissions.
> But no one can enter this file **( cd )** ... **EXCEPT ROOT!** 6. Create a file with permission 444. Try to edit in it and to remove it? Note what happened.
> Cannot write onto the file nor delete it. 7. What is the difference between the “x” permission for a file and for a directory?
> For **directories** means, that if we can access **( cd )** the directory
> for **files** means, that we cannot execute _(script)_ or edit _(write)_ file ... we can only read _(cat)_

---

### 1. Set the sticky bit on the newly created directory.

```bash
chmod +t test_directory
```

### 2. Set the setgui bit on the created directory

```bash
chmod g+s test_directory
```

### 3. Create multiple user accounts.

```bash
sudo adduser user1
sudo adduser user2
```

### 4. Allow these users to create files within the directory and directory.

```bash
chown :user1 test_directory
chmod g+rwx test_directory
```

### 5. Provide a clear output for the directory created.

```bash
ls -ld test_directory
```
