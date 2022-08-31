#include "prompt.h"
#include "headers.h"
#include "inputs.h"
#include "execute.h"

int main()
{
    init_shell();
    while (1)
    {
        prompt();
        char* line;
        line = get_input();

        char* com = strtok(line, ";\n");
        while (com != NULL)
        {

            int argc = 0;
            char** argv = parse_input(com, &argc);
            if (argc == 0)
            {
                com = strtok(NULL, ";\n");
                continue;
            }
            // printf("\n");
            // for (int i = 0; i < argc; i++)
            // {
            //     printf("%s ", argv[i]);
            // }
            // printf("\n");
            execute(argc, argv);
            com = strtok(NULL, ";\n");
        }
    }
}
