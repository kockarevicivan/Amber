namespace Weellab.VRMVC.Entities
{
    public class HotpResponseBody
    {
        public string _Content;

        public HotpResponseBody(string content)
        {
            if (content != null)
                _Content = content;
            else
                _Content = null;
        }
    }
}
