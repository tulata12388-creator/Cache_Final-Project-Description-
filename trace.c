#include "trace.h"
#include "cache_core.h"
#include "cache_operations.h"
#include "cache_output.h"
#include "cache_state.h"

#include <stdio.h>
#include <ctype.h>
#include <inttypes.h>

/*
line: dòng trong file trace.txt cần phân tích
address: địa chỉ được phân tích từ dòng trace.txt
has_address: biến cờ để xác định dòng trace có địa chỉ hay không
*/
TraceType parse_trace_line(const char *line, uint32_t *address, int *has_address)
{
  	// loại bỏ kí tự trắng ở đầu line 
    while (isspace((unsigned char)*line)) {
        line++;
    }
    if (*line == '\0') {
        return TRACE_LINE_BLANK;
    }

    unsigned int n; // Loại chức năng nào (n = 0,1,2,3,8,9)
    unsigned int parsed_address = 0; // địa chỉ (này tùy theo n khác nhau mà có thể có hay ko)
    char extra; // Biến kiểm tra dòng có chứa kí tự thừa không ? (thực ra sẽ không dùng làm gì hết)
    int fields = sscanf(line, "%u %x %c", &n, &parsed_address, &extra);
    //1: đọc được n, nhưng không đọc được địa chỉ.
    //2: đọc được n và địa chỉ dạng hexadecimal.
    //3: còn có thêm ký tự không mong đợi sau địa chỉ; code từ chối dòng này.
    if (fields < 1 || fields > 2) {
        return TRACE_LINE_INVALID;
    }

    *address = (uint32_t)parsed_address;
    if (fields == 2) {
        *has_address = 1;
    } else {
        *has_address = 0;
    }

    switch (n) {
        case TRACE_DATA_READ:
            if (fields != 2) {
                return TRACE_LINE_INVALID;
            }
        	break;
        case TRACE_DATA_WRITE:
            if (fields != 2) {
                return TRACE_LINE_INVALID;
            }
        	break;
        case TRACE_INSTRUCTION_FETCH:
            if (fields != 2) {
                return TRACE_LINE_INVALID;
            }
        	break;
        case TRACE_L2_EVICT:
            if (fields != 2) {
                return TRACE_LINE_INVALID;
            }
            break;
        case TRACE_CLEAR_CACHE:
            if (fields != 1) {
                return TRACE_LINE_INVALID;
            }
        	break;
        case TRACE_PRINT_CACHE:
            if (fields != 1) {
                return TRACE_LINE_INVALID;
            }
            break;
        default:
            return TRACE_LINE_INVALID;
    }
    return (TraceType)n;
}

const char *trace_type_name(TraceType type)
{
    switch (type) {
        case TRACE_DATA_READ:       	return "read data";
        case TRACE_DATA_WRITE:      	return "write data";
        case TRACE_INSTRUCTION_FETCH: 	return "instruction fetch";
        case TRACE_L2_EVICT: 			return "L2 evict";
        case TRACE_CLEAR_CACHE: 		return "clear cache";
        case TRACE_PRINT_CACHE: 		return "print cache";
        default: 						return "unknown";
    }
}

int read_text_file(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL){
        perror(filename); // in ra lỗi 
        return 1;
    }
    char line[1024];
    unsigned int line_number = 0;
    while (fgets(line, sizeof line, file) != NULL) // Đọc từng dòng 
    {
        uint32_t address;
        int has_address;
        TraceType n;
        line_number++;
        n = parse_trace_line(line, &address, &has_address);

        if (n == TRACE_LINE_BLANK) {
            continue;
        }
        if (n == TRACE_LINE_INVALID) {
            fprintf(stderr, "%s:%u: dong trace khong hop le\n", filename, line_number);
            fclose(file);
            return 1;
        }

        printf("n=%u: %s", (unsigned int)n, trace_type_name((TraceType)n));
        if (has_address)
        {
            printf(", address=0x%08" PRIx32, address);
        }
        putchar('\n');

        // Thuc thi lenh sau khi da in thong tin trace (n=...)
        switch (n) {
            case TRACE_DATA_READ:
                read_data(address);
                break;
            case TRACE_DATA_WRITE:
                write_data(address);
                break;
            case TRACE_INSTRUCTION_FETCH:
                fetch_instruction(address);
                break;
            case TRACE_L2_EVICT:
                evict_L2(address);
                break;
            case TRACE_CLEAR_CACHE:
                clear_cache();
                break;
            case TRACE_PRINT_CACHE:
                print_cache();
                break;
            default:
                // parse_trace_line da loc cac trace khong hop le
                break;
        }

        
        printf("\n====================================================================\n");
    }
    if (ferror(file)) {
        fprintf(stderr, "Loi khi doc file: %s\n", filename);
        fclose(file);
        return 1;
    }

    if (fclose(file) != 0) {
        perror(filename); // lỗi đóng file
        return 1;
    }
    print_statistics();

    return 0;
}