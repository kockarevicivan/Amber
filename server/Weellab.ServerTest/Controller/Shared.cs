
namespace Weellab.ServerTest.Controller
{
    using Weellab.VRMVC.Entities;

    public class Shared : VRController
    {
        public HotpResponse Error()
        {
            return View();
        }
    }
}
