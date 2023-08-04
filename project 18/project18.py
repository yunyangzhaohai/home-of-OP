from bitcoinrpc.authproxy import AuthServiceProxy, JSONRPCException

# 自行设置rpc_user、rpc_password、rpc_host和rpc_port
rpc_user = 'your_rpc_user'
rpc_password = 'your_rpc_password'
rpc_host = 'localhost'
rpc_port = 18332

# 创建RPC连接
rpc_connection = AuthServiceProxy(f"http://{rpc_user}:{rpc_password}@{rpc_host}:{rpc_port}")

def send_transaction():
    try:
        sender_address = rpc_connection.getnewaddress()
        rpc_connection.generatetoaddress(101, sender_address)
        receiver_address = rpc_connection.getnewaddress()
        unspent = rpc_connection.listunspent(1, 9999999, [sender_address])
        inputs = []
        for u in unspent:
            inputs.append({"txid": u['txid'], "vout": u['vout']})
        outputs = {receiver_address: 0.001}  # 发送 0.001 比特币到接收方地址
        raw_transaction = rpc_connection.createrawtransaction(inputs, outputs)
        signed_transaction = rpc_connection.signrawtransactionwithwallet(raw_transaction)
        txid = rpc_connection.sendrawtransaction(signed_transaction['hex'])
        print("Transaction sent! TXID: ", txid)
        
    except JSONRPCException as e:
        print(f"Error: {e.error['message']}")

def decode_transaction(txid):
    try:
        transaction = rpc_connection.getrawtransaction(txid, 1)
        total_input = 0
        total_output = 0

        for vin in transaction['vin']:
            input_tx = rpc_connection.getrawtransaction(vin['txid'], 1)
            input_vout = vin['vout']
            input_value = input_tx['vout'][input_vout]['value']
            total_input += input_value

        for vout in transaction['vout']:
            output_value = vout['value']
            total_output += output_value

        transaction_fee = total_input - total_output
        print("Transaction Details:")
        print(f"Total Input: {total_input} BTC")
        print(f"Total Output: {total_output} BTC")
        print(f"Transaction Fee: {transaction_fee} BTC")

    except JSONRPCException as e:
        print(f"Error: {e.error['message']}")

# 发送交易
send_transaction()

# 解析交易数据(自行输入)
txid = 'your_txid'
decode_transaction(txid)
