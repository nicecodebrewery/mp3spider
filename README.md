# mp3spider 🕷️

A simple C++ command-line tool to **read** and **rewrite** ID3v2 metadata tags in MP3 files — including title, artist, album, year, and publisher.


## 📦 Features

- ✅ View ID3v2 tags of any `.mp3` file
- 📝 Overwrite metadata fields (`TIT2`, `TPE1`, `TALB`, `TYER`, `TPUB`)
- 📂 Output to a new `.mp3` without touching original audio
- 🛡️ Safe: audio is untouched, only metadata is rewritten
- 🧹 Automatic cleanup of temporary tag data


## 🛠️ Build

You need a C++17 (or later) compiler:

```bash
make
make install
```

### Uninstalling

```bash
make uninstall
```

## 🚀 Usage

### 📖 Read metadata
```bash
./mp3spider input.mp3
```

Displays all available ID3v2.3 tag frames (like title, artist, album, etc.).



### ✍️ Write/Edit metadata
```bash
./mp3spider input.mp3 output.mp3 [title] [artist] [album] [year] [publisher]
```

Arguments are optional after output.mp3. Examples:

#### ✅ Only title
```bash
./mp3spider song.mp3 song_edited.mp3 "New Title"
```
#### ✅ Full metadata
```bash
./mp3spider song.mp3 final.mp3 "Bohemian Rhapsody" "Queen" "A Night at the Opera" "1975" "EMI"
```

## 🧪 Example

```bash
./mp3spider original.mp3 updated.mp3 "Imagine" "John Lennon" "Imagine" "1971" "Apple Records"
```

Produces updated.mp3 with updated metadata and the same audio as original.mp3.



## 🧼 Temp File Handling

A temporary file temp.mp3 is created to hold the new metadata tag and is automatically deleted after use.

If not deleted (e.g., file lock), you’ll see:
```bash
Warning: Temporary file 'temp.mp3' could not be deleted.
```



## 🗂 Project Structure
```bash
mp3spider/
├── src/
│   └── main.cpp          # CLI entry point
├── scripts/
│   └── tagfinder.cpp     # Tag parsing & writing logic
│   └── tagfinder.h
```



## ⚖️ License

MIT License — feel free to use and contribute!



## 🙌 Credits

Built by Vijay Satheesh. Made with 🧠 C++ and ❤️ for open-source audio tools.
