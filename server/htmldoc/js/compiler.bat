protoc -I..\..\proto\ --js_out=library=protocol,binary:. ..\..\proto\character.proto ..\..\proto\core.proto ..\..\proto\user.proto
python closure-library\closure\bin\build\depswriter.py --root_with_prefix="protobuf ../../../protobuf" --path_with_depspath="protocol.js ../../../protocol.js" --output_file=deps.js
::python C:\projects\thirdparty\js\closure-library\closure\bin\calcdeps.py -i protocol.js -i my.js -p C:\projects\thirdparty\js\protobuf -p C:\projects\thirdparty\js\closure-library -o compiled --compiler_jar C:\Users\KpoxaPy\bin\closure-compiler.jar --compiler_flags "--js_output_file min.js"
::ren "--js_output_file min.js" min.js
