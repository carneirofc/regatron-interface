#!/bin/bash
set -e
# Get Regatron dependencies
cd Regatron/
wget https://www.regatron.com/service/download/programming-interfaces/tcio-api.zip
unzip tcio-api.zip
rm -rf Examples/ API\ for\ DLL\ Version\ 3.56\ \(v2.5\,e\).pdf V3.87.00\ 30102019/ tcio-api.zip 'V3.80.00 30072014 (Linux)/doc/'
cd -
