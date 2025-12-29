#include "simple_shell.h"

/**
 * main - Simple Shell main function
 *
 * Return: Always 0
 */
int main(void)
{
    /* نسوي لوب لا نهائي عشان الشل يضل شغال */
    while (1)
    {
        /* نطبع العلامة بس لو كنا في الوضع التفاعلي */
        if (isatty(STDIN_FILENO))
            write(STDOUT_FILENO, "$ ", 2);

        /* هنا بنضيف كود القراءة والتنفيذ لاحقاً */
        break; /* مؤقتاً نكسر اللوب عشان ما يعلق */
    }

    return (0);
}
