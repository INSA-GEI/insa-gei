using System;
using Gtk;

namespace EntretienAuto
{
	public static class Menu
	{
		private static Gtk.Window parentWindow;

		public static void MenuInit (Gtk.Window parent)
		{
			parentWindow = parent;
		}

		public static void OnVoitureActionActivated (object sender, EventArgs e)
		{
			Console.WriteLine ("Objet: "+sender+" ,Event: "+e);
			Console.WriteLine ("Parent: " + parentWindow);
			//throw new NotImplementedException ();
			parentWindow.statusbar1.statusbarLabel.Text = "tutu";
		}

		public static void OnEntretienActionActivated (object sender, EventArgs e)
		{
			throw new NotImplementedException ();
		}
	}
}

