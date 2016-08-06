Instructions on how to add (or modify) files to the repository (repository name: AliceItsUpgradeLbnl/BASE_tests) - For linux and MacOs

Preliminary operations (one time):

1. create an ssh key (using your github email address)

  - Run command: ssh-keygen -t rsa -b 4096 -C "your_email@example.com"
  
  - Upon message "Enter a file in which to save the key (/Users/you/.ssh/id_rsa):" type "id_rsa_aiu" and press enter

  - Upon messages "Enter phrase ..." and "Enter same phrase again:" just press enter in either case

  - Two files (id_rsa_aiu and id_ras_aiu.pub) will be created in the current folder.
    Move them to folder "/home/username/.ssh/" (over linux) or "/Users/username/.ssh/" (over MacOs).
    "username" in the path is your username in your local machine.

  - Check whether ssh-agent is enabled with command: eval "$(ssh-agent -s)"

  - Add your ssh key to ssh-agent (on MacOs this command works until Terminal tab is closed): ssh-add ~/.ssh/id_rsa_aiu

2. add this ssh key to your github account:

  - Linux:
    - Make sure xclip is installed
    - Copy your ssh key to your clipboard: xclip -sel clip < ~/.ssh/id_rsa_aiu.pub
    - Go to Github website, log in and go to your profile logo -> Settings -> SSH and GPG keys and press New SSH key button
    - Give your key a "Title" and move to "Key" field than Ctrl-V your key
    - Press Add SSH key to confirm your key

  - MacOS
    - Make sure pbcopy is installed
    - Copy your ssh key to your clipboard: pbcopy ~/.ssh/id_rsa_aiu.pub
    - Go to Github website, log in and go to your profile logo -> Settings -> SSH and GPG keys and press New SSH key button
    - Give your key a "Title" and move to "Key" field than Command-V your key
    - Press Add SSH key to confirm your key


Standard workflow:

1. Download all the repository (this will create a folder named BASE_tests in the current folder):

  git clone git@github.com:AliceItsUpgradeLbnl/BASE_tests.git

2. Go to the repository folder:

  cd BASE_tests

3. Create and switch to branch branch_name:

  git checkout -b branch_name

4. Can check status of the branches running command (asterisk must be in branch_name highlighted in green):

  git branch -a 

5. Make the desired modifications to the repository files/folders (don't touch the hidden .git subfolders)

6. Check which files you added/modified (in red):

  git status

7. Append the new/modified files/folders (don't add *~ files) to the list of repository files (all new/mod files must be in green afterwards):

  git add relative_file_path/file_name

  or 

  git add relative_folder_path/folder_name

8. In case you removed some files/folders from the repository use the command:

  git rm relative_file_path/file_name

  or 

  git rm relative_folder_path/folder_name

9. Commit your changes (use a meaningful comment)

  git commit -m "meaningful_comment"

10. Push your branch (branch_name) to the remote repository location (origin must not be replaced)

  git push origin branch_name

11. You can now delete you repository local folder "BASE_tests"

12. Go to Github website, log in and go to BASE_tests repository.

13. Select branch branch_name and press "New pull request" button, add a comment and press "Create pull request".

14. Your pull request will be merged by the admin (Alberto/Giacomo, don't merge it yourself) and the branch will be deleted by him
