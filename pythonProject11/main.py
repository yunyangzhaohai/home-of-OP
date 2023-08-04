from Cryptodome.Hash import SHA256
from Cryptodome.PublicKey import ECC
from Cryptodome.Signature import DSS
from rfc6979 import generate_k
def sm2_sign(message, private_key):
    hash_message = SHA256.new(message)
    key_parameters = ECC.curves.CURVES["sm2p256v1"]
    key = ECC.construct(curve=key_parameters, d=private_key)
    signature_scheme = DSS.new(key, "fips-186-3")
    k = generate_k(hash_message.digest(), private_key, SHA256)
    return signature_scheme.sign(hash_message, k)

def sm2_verify(message, public_key, signature):
    hash_message = SHA256.new(message)
    key_parameters = ECC.curves.CURVES["sm2p256v1"]
    key = ECC.construct(curve=key_parameters, point_x=public_key[0], point_y=public_key[1])
    signature_scheme = DSS.new(key, "fips-186-3")
    try:
        signature_scheme.verify(hash_message, signature)
        return True
    except ValueError:
        return False
private_key = 0x1234567890abcdef  # 替换为您的私钥
public_key = (0x0123456789abcdef, 0xabcdef0123456789)  # 替换为您的公钥

message = b"Hello, world!"
signature = sm2_sign(message, private_key)
valid = sm2_verify(message, public_key, signature)

print("Signature valid:", valid)