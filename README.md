# sf

sf provides a simple frontend to compilers.

**Keybindings:**
- *Up / Down*: navigates though the error/warning messages
- *Enter:* opens the file at the location of the error/warning
- *q:* quits the application

**Config:**
- The default config is `vim [file] '+normal [line]G[col]|'`
- This can be edited in `src/config.h`

**Installation:**
```
# install dependencies (Debian 10)
sudo apt install libncurses5-dev libncursesw5-dev

# download repo
git clone https://github.com/llxor/sf

cd sf
make install
```

**Uninstall:**
```
make uninstall
```
