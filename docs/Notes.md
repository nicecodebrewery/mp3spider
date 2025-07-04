### Overview about tags of MP3 files

ID3 tags are used to store metadata in an mp3 file. So we should read it first.

- ID3v1 tags: These are located at the very end of the MP3 file. You can read the last 128 bytes of the file. If these bytes start with "TAG", then the file has an ID3v1 tag.
- ID3v2 tags: These are typically found at the beginning of the file. An ID3v2 tag is identified by the signature "ID3" followed by the version number. You would need to examine the initial bytes of the file to see if they match this pattern. 

