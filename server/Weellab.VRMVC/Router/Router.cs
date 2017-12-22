namespace Weellab.VRMVC.Router
{
    using Entities;
    using System;
    using System.IO;
    using System.Reflection;

    public static class Router
    {
        public static HotpResponse ReceiveRequest(string request)
        {
            if (request.StartsWith("/Static"))
            {
                string codeBase = Assembly.GetEntryAssembly().CodeBase;
                UriBuilder uri = new UriBuilder(codeBase);
                string path = Uri.UnescapeDataString(uri.Path);

                string pathFull = Path.GetFullPath(Path.Combine(path, @"../../../" + request));
                
                string content = File.ReadAllText(pathFull);

                return new HotpResponse(HotpStatus.OK, "application/obj", content);
            }
            else if (request.StartsWith("/Oss"))
            {
                string codeBase = Assembly.GetEntryAssembly().CodeBase;
                UriBuilder uri = new UriBuilder(codeBase);
                string path = Uri.UnescapeDataString(uri.Path);

                string pathFull = Path.GetFullPath(Path.Combine(path, @"../../../" + request));

                string content = File.ReadAllText(pathFull);

                //parse oss with the parser

                return new HotpResponse(HotpStatus.OK, "text/oss", content);
            }
            else
            {
                string[] splitted = request.Split('/');
                string domain = splitted[0];
                string controller = splitted[1];
                string method = splitted[2];

                return Invoke(controller, method);
            }
        }

        private static HotpResponse Invoke(string typeName, string methodName, object[] parameters = null)
        {
            Assembly asm = Assembly.GetEntryAssembly();

            string fullName = String.Format("{0}.Controller.{1}", asm.FullName.Split(',')[0], typeName).ToLower();
            Type type = asm.GetType(fullName, true, true);
            object instance = Activator.CreateInstance(type);
            MethodInfo method = type.GetMethod(methodName, BindingFlags.IgnoreCase | BindingFlags.Public | BindingFlags.Instance);

            HotpResponse response;

            try
            {
                response = (HotpResponse)method.Invoke(instance, parameters);
            }
            catch (Exception ex)
            {
                string errClass = String.Format("{0}.Controller.{1}", asm.FullName.Split(',')[0], "Shared").ToLower();
                Type errType = asm.GetType(errClass, true, true);
                object errInstance = Activator.CreateInstance(errType);
                MethodInfo errMethod = type.GetMethod("Error", BindingFlags.IgnoreCase | BindingFlags.Public | BindingFlags.Instance);

                response = (HotpResponse)errMethod.Invoke(errInstance, null);
            }
            

            return response;
        }
    }
}
