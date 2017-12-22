
namespace Weellab.VRMVC.Server
{
    using System;
    using System.Collections.Generic;
    using System.Net;
    using System.Net.Sockets;
    using System.Text;
    using Router;
    using Entities;
    using Newtonsoft.Json.Linq;
    public class Server
    {
        private static byte[] _buffer = new byte[1024];
        private static Socket _socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        private static List<Socket> _clientSockets = new List<Socket>();

        public static void SetupServer(string ip, string port)
        {
            _socket.Bind(new IPEndPoint(IPAddress.Parse(ip), int.Parse(port)));
            _socket.Listen(1);
            _socket.BeginAccept(new AsyncCallback(AcceptCallback), null);
        }

        private static void AcceptCallback(IAsyncResult AR)
        {
            Socket socket = _socket.EndAccept(AR);
            _clientSockets.Add(socket);
            Console.WriteLine("Client connected.");
            socket.BeginReceive(_buffer, 0, _buffer.Length, SocketFlags.None, new AsyncCallback(ReceiveCallback), socket);
            _socket.BeginAccept(new AsyncCallback(AcceptCallback), null);
        }

        private static void ReceiveCallback(IAsyncResult AR)
        {
            Socket socket = (Socket)AR.AsyncState;

            try
            {
                int received = socket.EndReceive(AR);
                byte[] dataBuf = new byte[received];
                Array.Copy(_buffer, dataBuf, received);

                string text = Encoding.ASCII.GetString(dataBuf);

                byte[] data;
                byte[] zdata;

                if (String.IsNullOrEmpty(text))
                {
                    data = Encoding.ASCII.GetBytes("");

                    zdata = new byte[data.Length + 1];
                    data.CopyTo(zdata, 0);

                    socket.BeginSend(zdata, 0, zdata.Length, SocketFlags.None, new AsyncCallback(SendCallback), socket);
                }
                else
                {
                    HotpResponse response = Router.ReceiveRequest(text);
                    string jsonSstr = JObject.FromObject(response).ToString();

                    data = Encoding.ASCII.GetBytes(JObject.FromObject(response).ToString());

                    zdata = new byte[data.Length + 1];
                    data.CopyTo(zdata, 0);
                }
                
                socket.BeginSend(zdata, 0, zdata.Length, SocketFlags.None, new AsyncCallback(SendCallback), socket);

                socket.BeginReceive(_buffer, 0, _buffer.Length, SocketFlags.None, new AsyncCallback(ReceiveCallback), socket);
            }
            catch (SocketException se)
            {
                Console.WriteLine("Socket closed by client. Disposing connection...");
                socket.Close();
            }
        }

        private static void SendCallback(IAsyncResult AR)
        {
            Socket socket = (Socket)AR.AsyncState;
            socket.EndSend(AR);
        }
    }
}
