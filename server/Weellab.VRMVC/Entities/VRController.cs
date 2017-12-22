
namespace Weellab.VRMVC.Entities
{
    using System;
    using System.Diagnostics;
    using System.IO;
    using System.Reflection;
    using System.Runtime.CompilerServices;

    public class VRController
    {
        private string AssemblyDirectory
        {
            get
            {
                string codeBase = Assembly.GetEntryAssembly().CodeBase;
                UriBuilder uri = new UriBuilder(codeBase);
                string path = Uri.UnescapeDataString(uri.Path);
                return Path.GetDirectoryName(path);
            }
        }

        [MethodImpl(MethodImplOptions.NoInlining)]
        public HotpResponse View(object model = null)
        {
            StackTrace stackTrace = new StackTrace();
            StackFrame stackFrame = stackTrace.GetFrame(1);

            string fileContent = File.ReadAllText(@"../../View/" + stackFrame.GetMethod().ReflectedType.Name + "/" + stackFrame.GetMethod().Name + ".homl");

            HotpResponse response = new HotpResponse(HotpStatus.OK, HotpMimeType.HOML, fileContent);

            return response;
        }

        [MethodImpl(MethodImplOptions.NoInlining)]
        public HotpResponse View(string method, object model = null)
        {
            StackTrace stackTrace = new StackTrace();
            StackFrame stackFrame = stackTrace.GetFrame(1);

            string fileContent = File.ReadAllText(@"../../View/" + stackFrame.GetMethod().ReflectedType.Name + "/" + method + ".homl");

            HotpResponse response = new HotpResponse(HotpStatus.OK, HotpMimeType.HOML, fileContent);

            return response;
        }

        public HotpResponse View(string method, string controller, object model = null)
        {
            StackTrace stackTrace = new StackTrace();
            StackFrame stackFrame = stackTrace.GetFrame(1);

            string fileContent = File.ReadAllText(@"../../View/" + controller + "/" + method + ".homl");

            HotpResponse response = new HotpResponse(HotpStatus.OK, HotpMimeType.HOML, fileContent);

            return response;
        }
        
        [MethodImpl(MethodImplOptions.NoInlining)]
        public HotpResponse RedirectToAction(string method, object[] parameters = null)
        {
            StackTrace stackTrace = new StackTrace();
            StackFrame stackFrame = stackTrace.GetFrame(1);

            Assembly asm = Assembly.GetEntryAssembly();

            string fullName = String.Format("{0}.Controller.{1}", asm.FullName.Split(',')[0], stackFrame.GetMethod().ReflectedType.Name).ToLower();
            Type type = asm.GetType(fullName, true, true);
            object instance = Activator.CreateInstance(type);
            MethodInfo methodInfo = type.GetMethod(method, BindingFlags.IgnoreCase | BindingFlags.Public | BindingFlags.Instance);
            HotpResponse response = (HotpResponse)methodInfo.Invoke(instance, parameters);

            return response;
        }

        [MethodImpl(MethodImplOptions.NoInlining)]
        public HotpResponse RedirectToAction(string method, string controller, object[] parameters = null)
        {
            Assembly asm = Assembly.GetEntryAssembly();

            string fullName = String.Format("{0}.Controller.{1}", asm.FullName.Split(',')[0], controller).ToLower();
            Type type = asm.GetType(fullName, true, true);
            object instance = Activator.CreateInstance(type);
            MethodInfo methodInfo = type.GetMethod(method, BindingFlags.IgnoreCase | BindingFlags.Public | BindingFlags.Instance);
            HotpResponse response = (HotpResponse)methodInfo.Invoke(instance, parameters);

            return response;
        }
    }
}
