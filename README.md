# HOW TO RUN

1. Move `server.c` in the directory you want to transfer files from:

   ```bash
   gcc -o server server.c
   ```

   Run the following command

   ```bash
   ./server
   ```

2. Move `client.c` in the directory you want to transfer files to:

   ```bash
   gcc -o client client.c
   ```

   Run the following command

   ```bash
   ./client
   ```

## HOW TO USE

1. `get` :

   ```bash
   Usage: get <filename-1> ... <filename-N>
   ```

2. `exit`:

   ```bash
   Usage: exit
   ```
