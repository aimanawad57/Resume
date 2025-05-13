from operator import truediv
import json
import sys

MOD_ENCRYPT_NUM = 26
W1_MULTIPLY = 2
W1_MAX = 8

class Enigma:
    def __init__(self, hash_map, wheels, reflector_map):
        self.hash_map = hash_map
        self.reverse_hash_map = {v: k for k, v in hash_map.items()}  #Create reverse map
        self.wheels = wheels
        self.reflector_map = reflector_map
        self.org_wheels = wheels.copy()


    def encrypt(self, message):
        encrypted_message = ""
        num_of_encrypted = 0
        for index, char in enumerate(message):
            if not char.islower():
                encrypted_message += char
                self.update_wheels(num_of_encrypted)
                continue
            # Step 1
            i = self.hash_map[char]
            # Step 2
            temp_num = (W1_MULTIPLY * self.wheels[0] - self.wheels[1] + self.wheels[2]) % MOD_ENCRYPT_NUM
            if temp_num != 0:
                i += temp_num
            else:
                i += 1
            # Step 3
            i = i % MOD_ENCRYPT_NUM
            # Step 4
            c1 = self.reverse_hash_map[i]
            # Step 5
            c2 = self.reflector_map[c1]
            # Step 6
            i = self.hash_map[c2]
            # Step 7
            if temp_num != 0:
                i -= temp_num
            else:
                i -= 1
            # Step 8
            i = i % MOD_ENCRYPT_NUM
            # Step 9
            c3 = self.reverse_hash_map[i]

            encrypted_message += c3

            num_of_encrypted += 1
            self.update_wheels(num_of_encrypted)

        self.wheels = self.org_wheels.copy()
        return encrypted_message

    def update_wheels(self,num_of_encrypted):
        self.wheels[0] += 1
        if self.wheels[0] > W1_MAX:
            self.wheels[0] = 1

        if (num_of_encrypted % 2) == 0:
            self.wheels[1] *= 2
        else:
            self.wheels[1] -= 1

        if (num_of_encrypted % 10) == 0:
            self.wheels[2] = 10
        elif (num_of_encrypted % 3) == 0:
            self.wheels[2] = 5
        else:
            self.wheels[2] = 0


class JSONFileError(Exception):
    pass

def load_enigma_from_path(path):
    try:
        with open(path,'r') as file:
            data = json.load(file)

            hash_map = data['hash_map']
            wheels = data['wheels']
            reflector_map = data['reflector_map']

            return Enigma(hash_map, wheels, reflector_map)

    except (FileNotFoundError, json.decoder.JSONDecodeError) as e:
            raise JSONFileError(f"Error reading JSON file: {path}") from e

#Print functions

def print_usage():
    print("Usage: python3 enigma.py -c <config_file> -i <input_file> -o <output_file>", file=sys.stderr)
def print_runerror():
    print("The enigma script has encountered an error", file=sys.stderr)


def main():
    try:
        config_file = None
        input_file = None
        output_file = None
        if len(sys.argv) not in [5, 7]: #Possible number of arguments.
            print_usage()
            sys.exit(1)

        args = sys.argv
        valid_flags = {'-i','-c','-o'}
        i=1
        while i < len(args):
            if args[i] in valid_flags:
                if args[i] == '-c' and i+1 < len(args):
                    config_file = args[i+1]
                elif args[i] == '-o' and i+1 < len(args):
                    output_file = args[i+1]
                elif args[i] == '-i' and i+1 < len(args):
                    input_file = args[i+1]
                else:
                    print_usage()
                    sys.exit(1)
            else:
                print_usage()
                sys.exit(1)
            i+=2 #checks the next flag

        if not config_file or not input_file:
            print_usage()
            sys.exit(1)
        try:
            enigma = load_enigma_from_path(config_file)
        except JSONFileError:
            print_runerror()
            sys.exit(1)


        encrypted_messages = []
        try:
            with open(input_file, 'r') as file:
                for line in file:
                    encrypted_message = enigma.encrypt(line)
                    encrypted_messages.append(encrypted_message)
        except IOError:
            print_runerror()
            sys.exit(1)

        encrypted_output = "\n".join(encrypted_messages)

        if output_file:
            try:
                with open(output_file, 'w') as file:
                    file.write(encrypted_output)
            except IOError:
                print_runerror()
                sys.exit(1)
        else:
            print(encrypted_output)
    except Exception :
        print_runerror()
        sys.exit(1)


if __name__ == "__main__":
    main()
