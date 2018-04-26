This is just a shell plugin. Add in your stuff

1) Setup your environment variables as shown in the writeup for the assignment
Basically, you should have defined:
CUSTOM_DSO_PATH -> This is the place where the plugins are built and read by houdini. eg: 
H16_PATH -> Houdini installation path. eg: C:\Program Files\Side Effects Software\Houdini 16.5.268

2) I have setup the VS project so that it can just be opened and built provided that the above two environment variables are defined

3) On Windows, Houdini plugin is a .dll, on Macos .dylib(??) and on linux .so

4) Here is a nice way to package your entire node network and ship it to customers:
Houdini Digital asset : https://vimeo.com/116797081

5) Enjoyee and best of luck!

