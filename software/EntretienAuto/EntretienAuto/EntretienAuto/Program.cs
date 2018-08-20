using System;
using Gtk;

namespace EntretienAuto
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			Application.Init ();
			FenetrePrincipale win = new FenetrePrincipale ();
			win.Show ();
			Application.Run ();
		}
	}
}
