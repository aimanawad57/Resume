

my_str = "The first one is the easiest!"

new_str = [ord(elem) for elem in my_str]


print(*{x:f"{x}^{x} is {x ** x}\n" for x in range(10,0,-1)}.values())

print(*[f"The ASCII number {i} represent the char {chr(i)}\n"for i  in range(ord('A'), ord('z')+1, 2) if chr(i).isalpha()])

list_c = [80, 121, 116, 104, 111, 110, 32, 105, 115, 32, 102, 117, 110, 33]


print("".join([chr(num) for num in list_c]))
