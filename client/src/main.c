#include "client.h"

t_main_struct *main_struct;

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    dyad_init();

    main_struct = init_main_struct(argc, argv);

    authorization(NULL, main_struct);

    gtk_main();
}
