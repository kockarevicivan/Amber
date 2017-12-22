
namespace Weellab.ServerTest
{
    using System;
    using VRMVC.Server;
    using VRMVC.Router;
    using VRMVC.Entities;
    using Newtonsoft.Json.Linq;
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Listening...");
            Server.SetupServer("127.0.0.1", "60000");

            Console.ReadKey();

            //HotpResponse response = Router.ReceiveRequest("www.weellab.com/home/index");

            //Console.WriteLine(response._Body._Content);

            //Console.WriteLine(JObject.FromObject(response));
        }
    }
}

