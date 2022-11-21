#include <stdio.h>

#include "src/log_info/log_errors.h"
#include "src/Generals_func/generals.h"

#include "calculator.h"

int main ()
{   
    #ifdef USE_LOG
        
        if (Open_logs_file ())
            return OPEN_FILE_LOG_ERR;

    #endif


    FILE *fpin = Open_file_ptr ("input.txt", "r");
    
    if (Check_nullptr (fpin)) return PROCESS_ERROR (ERR_FILE_OPEN, "file not opened for reading\n");

    char expression[Max_buffer_size] = "";
    int scan_res = fscanf (fpin, "%s", expression);

    if (scan_res != 1)
    {
        Err_report ("The number of arguments was considered incorrect\n");
        return -1;
    }


    int ans = Parce_math_expression (expression);

    printf ("%s = %d", expression, ans);
    
    
    
    #ifdef USE_LOG
        
        if (Close_logs_file ())
            return CLOSE_FILE_LOG_ERR;

    #endif

    return 0;
}