#include <iostream>
#include <memory>

void sum(int &num1, int &num2)
{
    num1 = num1 + num2;
    std::cout << num1 << " - " << num2 << std::endl;
}

int main()
{
    // char x[] = {'a', 'b', 'c'};
    // char *p1 = x;
    //
    // printf("%p -- %p \n", &x, p1);

    int x = 10, y = 20;
    sum(x, y);
    //    std::cout << sizeof(void) << std::endl;

    /* Unique Pointer */
    /* */
    std::unique_ptr<int> x2(new int(47));
    std::unique_ptr<int> x4(new int);
    int *x3 = new int(30);
    printf("%d\n", *x2);
    x2.reset(x3);
    printf("%d\n", *x2);

    x4 = std::move(x2);
    printf("%d\n", *x4);
    // printf("%d\n", *x2);     --> Will through segmentation fault

    std::unique_ptr<int> foo;
    foo = std::unique_ptr<int>(new int(10));
    // printf("%p -- %p \n", foo, foo.get());   -->     [ foo ] cannot be used explicitly (error!)
    printf("%p -- %p\n", foo.get(), &foo);

    std::shared_ptr<int> sh1(new int(12345));
    printf("%ld \n", sh1.use_count());
    int *nSh1 = sh1.get(); // doesn't share ownership (count)
    std::shared_ptr<int> sh2 = sh1;
    printf("%ld -- %ld\n", sh1.use_count(), sh2.use_count());

    return 0;
}