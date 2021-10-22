1.  shellcode.c
    In this file i have just initiated command prompt and took the next command.
    
    FUNCITONS:-
        clear():
            This function will clear the output screen.
        file_read():
            This function read the history file into my history array where I am storing my history commands.
        na_name():
            This function will create a string "/home/username" for me.
        code_history():
            This function will create print the history values for me.
        args_exec():
            This function will set the function according to the input.
        enter():
            This function will take the input for me.
        hostname():
            This function will give me the hostname.
        shell():
            This function will print the command prompt at each step.
        has_output_redirection():
            This function will check if there is output redirection '>' in the string passed to it.
        has_input_redirection():
            This function will check if there is input redirection '<' in the string passed to it.  
        has_append():
            This function will check if there is append '>>' in the string passed to it.
        has_pipe():
            This function will check if there is a pipe in the string passed to it.
        o_r_parse():
            This function will parse the output side of a redirection.
        i_r_parse():
            This function will parsed the input side of a redirection.
        print_jobs():
            This function will print all the jobs present in background.
        fg_job():
            This function will bring a job running in background to foreground.
        bg_job():
            This function will execute bg command similar to the bash shell.
        k_job():
            This function will send the provided signal to the given job.
        overkill_jobs():
            This function will finish all the process present in the background.
        handle():
            This function handles the siganls.
            

2.  code_cd.c
        This code will help in executing cd command similar to the original terminal.

3.  code_echo.c
        This code will help in executing echo command double quotes will also work but 
        \n,\t are not supported.

4.  code_exit.c
        This code will help in exiting the shell and it will store the history values in the history file created so that we can access previous history values as well;

5.  code_ls.c
        This code will help in executing ls command similar to the original terminal
        -l,-a,-al/la flags are supported.

6.  code_pinfo.c
        This  code will help in executing pinfo command printing the pid,status,virtual memory,executable path.

7.  code_pwd.c
        This code will help in printing the present working directory.

8.  parse_input.c
        This code will separate the input into different tokens and it will help in examining the input command entered by user.

9.  perm_check.c
        This code will check the permissions of the file in a directory and helps in printing permissions,user,group,date and time,no. of bytes.

10. pwd.c
        This code will give the present working directory.

11. Makefile
        In this file I am compiling all my .c files and then you just have to enter "make" to compile all the files in a single go.

12. code_redirect.c
        This function will implement redirection.

13. execute_pipe.c
        This function will execute pipe without redirection.

14. execute_pipe_n_r.c
        This function will execute pipe with redirection.

15. rm_space.c
        This function will remove all the starting spaces from a given input.

16. rm_t_space.c
        This function will remove all the trailing spaces from a given input.

17. var_set.c
        This function will set environment variables.

18. var_unset.c
        This function will unset environment variables.

NOTE:-
    I've also created code_cd.h,code_echo.h,code_exit.h,code_history.h,code_ls.h,code_pinfo.h,code_pwd.h,pwd.h,perm_check.h,parse_input.h header files.