using Aniliberty.Unfolded.Models.MainMenus;

namespace Aniliberty.Unfolded.Routes
{

	public static class MainMenu
	{

		private static string m_userName = "";

		private static string m_userAvatar = "";

		private static readonly List<MainMenuModel> m_mainMenu = [];

		public static void RegisterRoutes(WebApplication app)
		{
			m_mainMenu.AddRange(
				[
					new MainMenuModel
					{
						Name = "Каталог релизов",
						Icon = ""
					},
					new MainMenuModel
					{
						Name = "Видеоплеер",
						Icon = ""
					},
					new MainMenuModel
					{
						Name = "Моя Анилибрия",
						Icon = ""
					},
					new MainMenuModel
					{
						Name = "Кинозал",
						Icon = ""
					},
					new MainMenuModel
					{
						Name = "Связанные релизы",
						Icon = ""
					},
					new MainMenuModel
					{
						Name = "Torrent клиент",
						Icon = ""
					},
					new MainMenuModel
					{
						Name = "Авторизация",
						Icon = ""
					}
				]
		);

			app.MapGet("/mainmenu/items", () => Items());
			app.MapGet("/mainmenu/user", () => User());
		}

		public static void SetUser(string name, string avatar)
		{
			m_userAvatar = avatar;
			m_userName = name;

			var authorizationItem = m_mainMenu
				.FirstOrDefault(a => a.Name == "Авторизация");
			if (authorizationItem != null) m_mainMenu.Remove(authorizationItem);

		}

		public static void RemoveUser()
		{
			m_userAvatar = "";
			m_userName = "";

			m_mainMenu.Add(
				new MainMenuModel
				{
					Name = "Авторизация",
					Icon = ""
				}
			);
		}

		private static MainMenuUserModel User() =>
			new()
			{
				Avatar = m_userAvatar,
				Name = m_userName,
			};

		private static IEnumerable<MainMenuModel> Items() => m_mainMenu;

	}

}
