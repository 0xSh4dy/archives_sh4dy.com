# .NET
.NET is a free, cross platform, open source development platform for building applications. With .NET, you can use multiple languages such as C#, F# and Visual Basic to build web, mobile, desktop applications and much more.

## Common Language Runtime (CLR)
It is the virtual machine component of the .NET framework that manages the execution of .NET programs.
It uses a JIT (Just-in time) compiler to generate machine code which is subsequently executed by the CPU.

## .NET Core vs .NET
.NET core is the platform on which frameworks such as ASP.NET and the Universal Windows Framework rely on and extend the .NET core functionalities. It is compatible with Linux and MAC as well whereas .NET framework is a fully fledged development framework which provides all the basic requirements for development of applications. .NET framework is compatible only with windows.

## Compiling C#
C# is not compiled into machine code directly but is compiled into an intermediate code known as the `Common Intermediate Language (CIL) or Intermediate Language Code (ILC or IL)`. This IL code can run on any operating system because C# is a platform independent language. The virtual machine component known as `Common Language Runtime (CLR)` does the task of converting the IL code into machine instructions. This is a JIT (Just in time) compiler (compiles code at runtime). Once the programs are compiled, they are physically packaged into assemblies. An assembly is a file that contains one or more namespaces and classes. As the number of classes and namespaces grows, it is separated by related namespaces into separate assemblies. Assemblies have the file extension `.dll` or `.exe`. 


