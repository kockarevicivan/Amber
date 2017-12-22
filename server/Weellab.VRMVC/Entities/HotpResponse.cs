namespace Weellab.VRMVC.Entities
{
    public class HotpResponse
    {
        public HotpResponseHead _Head;
        public HotpResponseBody _Body;

        public HotpResponse(HotpStatus status, string contentType, string bodyContent)
        {
            _Body = new HotpResponseBody(bodyContent);
            _Head = new HotpResponseHead(status, bodyContent.Length, contentType);
        }
    }
}
