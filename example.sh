curl -X 'POST' -H "Transfer-Encoding: chunked" -H 'Expect:' --data @70kb.txt http://127.0.0.1:8888/directory/youpi.bla
curl -X 'POST' -H "Transfer-Encoding: chunked" -H 'Expect:' --data @700kb.txt http://127.0.0.1:8888/directory/youpi.bla
curl -X 'POST' -H "Content-Type: application/json"  --data '{"who?":"Cyberserv!"}'  http://127.0.0.1:8888/directory/youpi.bla