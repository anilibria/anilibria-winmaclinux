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
						Icon = "releases"
					},
					new MainMenuModel
					{
						Name = "Видеоплеер",
						Icon = "videoplayer"
					},
					new MainMenuModel
					{
						Name = "Мои Подборки",
						Icon = "mypage"
					},
					new MainMenuModel
					{
						Name = "Кинозал",
						Icon = "cinemahall"
					},
					new MainMenuModel
					{
						Name = "Связанные релизы",
						Icon = "series"
					},
					new MainMenuModel
					{
						Name = "Torrent клиент",
						Icon = "torrent"
					},
					new MainMenuModel
					{
						Name = "Обслуживание",
						Icon = "maintenance"
					},
					new MainMenuModel
					{
						Name = "Авторизация",
						Icon = "authorization"
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
