#include <iostream>
#include <node.h>

namespace other
{
  const char *globalNumber = "nombre apellido edad curso ciclo";
  const char *globalNumber2 = "correcta inserción";
}

namespace interface
{

  using v8::Context;
  using v8::FunctionCallbackInfo;
  using v8::Isolate;
  using v8::Local;
  using v8::Object;
  using v8::String;
  using v8::Value;

  void CreateObject(const FunctionCallbackInfo<Value> &args)
  {
    Isolate *isolate = args.GetIsolate();

    args.GetReturnValue().Set(String::NewFromUtf8(
                                  isolate, other::globalNumber)
                                  .ToLocalChecked());
  }

  void Insert(const FunctionCallbackInfo<Value> &args)
  {
    Isolate *isolate = args.GetIsolate();

    args.GetReturnValue().Set(String::NewFromUtf8(
                                  isolate, other::globalNumber2)
                                  .ToLocalChecked());
  }

  void Init(Local<Object> module)
  {
    NODE_SET_METHOD(module, "expor", CreateObject);

    NODE_SET_METHOD(module, "insert", Insert);
  }

  NODE_MODULE(NODE_GYP_MODULE_NAME, Init)

} // namespace demo