#include "client.h"

t_main_struct *main_struct;

int main(int argc, char *argv[]) {
    if (argc < 3) {
        write(STDERR_FILENO, "usage: ./uchat ip_adress port\n", 31);
        exit(EXIT_FAILURE);
    }

    gtk_init(&argc, &argv);
    dyad_init();

    main_struct = init_main_struct(argc, argv);

    authorization(NULL, main_struct);

    gtk_main();
}
