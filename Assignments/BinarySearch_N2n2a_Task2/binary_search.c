#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define FOUND       1
#define NOT_FOUND   0

uint8_t binary_search(uint16_t arr[], uint16_t size, uint16_t element);

uint16_t cmp( const void* a, const void* b)
{
    return ( *(uint16_t*)a - *(uint16_t*)b );
}

void main()
{
    uint16_t myArr[] = {2, 7, 16, 14, 22, 37, 5, 7, 28, 1, 0, 6, 15, 14, 9};
    uint16_t size = sizeof(myArr) / sizeof(myArr[0]);
    uint16_t input;
    printf("Array:\n");
    for(uint16_t i = 0; i < size; i++)
        printf("%hu ", myArr[i]);
    printf("\nElement to search for >> ");
    scanf("%hu", &input);
    qsort(myArr, size, sizeof(uint16_t), cmp);
    uint8_t response = binary_search(myArr, size, input);
    if( response == FOUND )
    {
        printf("Found!\n");
    }
    else
    {
        printf("Not Found!\n");
    }
}

uint8_t binary_search(uint16_t arr[], uint16_t size, uint16_t element)
{
    uint8_t ret = NOT_FOUND;

    uint16_t start = 0;
    uint16_t end = size-1;
    uint16_t mid = 0;
    while(start <= end)
    {
        mid = start + (end - start) / 2;
        if( arr[mid] == element )
        {
            ret = FOUND;
            break;
        }
        else if( arr[mid] < element )
        {
            start = mid + 1;
        }
        else
        {
            end = mid - 1;
        }
    }

    return ret;
}
