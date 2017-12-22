
namespace Weellab.ServerTest.Controller
{
    using VRMVC.Entities;

    public class Home : VRController
    {
        public HotpResponse Index()
        {
            return View();
        }

        public HotpResponse Test()
        {
            return View();
        }
    }
}
