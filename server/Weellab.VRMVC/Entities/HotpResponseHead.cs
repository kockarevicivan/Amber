
namespace Weellab.VRMVC.Entities
{
    public class HotpResponseHead
    {
        public HotpStatus _Status;
        public int _ContentLength;
        public string _MimeType;

        public HotpResponseHead(HotpStatus status, int contentLength, string mimeType)
        {
            _Status = status;
            _ContentLength = contentLength;
            _MimeType = mimeType;
        }
    }
}