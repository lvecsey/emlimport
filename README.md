This project showcases the emlimport.c file and it has the following use case:

Messages are often written to an s3 bucket, however they don't have an .eml extension. So to remedy this and to be able to read .eml files with an email reader such as Thunderbird, we perform the actions below.

*Preparing*

First do an 'aws s3 sync' of your bucket to a directory path on your system.

You probably want to put that in a crontab entry, for periodic refreshing of the input directory (described below) throughout the day.

*Compile and run*

```console
make emlimport
./emlimport /mnt/synced/s3bucket_mail /mnt/mail/emlfiles
```

You can run the above emlimport command repeatedly with the given input, and
corresponding output directory (directly after doing an aws s3 sync) to convert any newly synced files to .eml files in the output directory.

A critbit file is placed in /mnt/mail/emlfiles as critlist.txt and is autogenerated and maintained. You don't need to modify it. It contains line by line a list of base file names from the input directory, so that we know which files to skip when processing any further .eml files for output.

*Thunderbird usage*

Use the extension called ImportExportTools NG, and use a right-click to select Import all messages from a directory, then just from the directory. It will read each .eml file and place it into your Inbox or the selected mail folder.


