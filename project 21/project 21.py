import hashlib
import random

# 选择大素数p和生成元g
p = 7919
g = 7

def generate_key_pair():
    x = random.randint(1, p-1)
    Y = pow(g, x, p)
    return x, Y

def sign_message(private_key, message):
    k = random.randint(1, p-1)
    R = pow(g, k, p)
    
    hash_input = str(message) + str(R)
    e = int(hashlib.sha256(hash_input.encode()).hexdigest(), 16)
    
    s = (k + (private_key * e)) % (p-1)
    return R, s

def sign_batch(private_key, messages):
    R_list = []
    s_sum = 0
    
    for message in messages:
        k = random.randint(1, p-1)
        R = pow(g, k, p)
        
        hash_input = str(message) + str(R)
        e = int(hashlib.sha256(hash_input.encode()).hexdigest(), 16)
        
        s = (k + (private_key * e)) % (p-1)
        
        R_list.append(R)
        s_sum = (s_sum + s) % (p-1)
    
    return R_list, s_sum

def verify_signature(public_key, message, signature):
    R, s = signature
    
    hash_input = str(message) + str(R)
    e = int(hashlib.sha256(hash_input.encode()).hexdigest(), 16)
    
    v = (pow(g, s, p) * pow(public_key, e, p)) % p
    
    if v == R:
        return True
    else:
        return False

def main():
    private_key, public_key = generate_key_pair()
    
    message = "Hello, world!"
    signature = sign_message(private_key, message)
    valid = verify_signature(public_key, message, signature)
    print("Single Signature Valid:", valid)
    
    messages = ["This", "is", "a", "batch", "of", "messages"]
    r_list, s_sum = sign_batch(private_key, messages)
    valid = all([verify_signature(public_key, m, (r, s_sum)) for m, r in zip(messages, r_list)])
    print("Batch Signature Valid:", valid)

if __name__ == "__main__":
    main()
