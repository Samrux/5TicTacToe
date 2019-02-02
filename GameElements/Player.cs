using System;

namespace TTT5.GameElements
{
    /// <summary>
    /// Represents one of the players in the game.
    /// Acts as a wrapper for <see cref="int"/>, like an enum.
    /// Valid values range from 0 to 3.
    /// </summary>
    public readonly struct Player : IEquatable<Player>, IEquatable<int>
    {
        public static readonly Player
            None = 0,
            One = 1,
            Two = 2,
            Tie = 3;


        private readonly int value;

        /// <summary>Returns the opposing player.</summary>
        public Player Opponent => this == One ? Two : One;

        /// <summary>Returns this player's Tic-Tac-Toe symbol.</summary>
        public char Symbol => Symbols[value];

        /// <summary>Returns this player's pointer appearance used to choose a cell.</summary>
        public string Pointer => Pointers[value];

        /// <summary>Returns the appearance of the turn title for this player.</summary>
        public string TurnTitle => TurnTitles[value];

        /// <summary>Returns the appearance of the win screen title for this player.</summary>
        public string WinTitle => WinTitles[value];

        /// <summary>Returns the color that represents this player.</summary>
        public ConsoleColor Color => Colors[value];

        /// <summary>Returns the background color that represents this player.</summary>
        public ConsoleColor BgColor => BgColors[value];

        /// <summary>Returns the console beep tone for this player.</summary>
        public int Tone => Tones[value];

        /// <summary>The name of this player.</summary>
        public override string ToString() => Names[value];


        private Player(int value)
        {
            this.value = value;
        }


        public static implicit operator int(Player player) => player.value;

        public static implicit operator Player(int value) => new Player(value);

        public static bool operator ==(Player left, Player right) => left.value == right.value;

        public static bool operator !=(Player left, Player right) => !(left == right);

        public bool Equals(Player player) => this == player;

        public bool Equals(int i) => value == i;

        public override bool Equals(object obj) => ReferenceEquals(this, obj);

        public override int GetHashCode() => value.GetHashCode();


        private static readonly char[] Symbols =
        {
            ' ', 'X', 'O', ' ',
        };

        private static readonly string[] Pointers =
        {
            "", "> <", "( )", null,
        };

        private static readonly string[] TurnTitles =
        {
            "", "+>> Player Red's turn <<+", "/(( Player Blu's turn ))\\", null,
        };

        private static readonly string[] WinTitles =
        {
            "", " <@+ Player Red won! +@> ", " ~@{ Player Blu won! }@~ ", "   -=] It's a tie! [=-   ",
        };

        private static readonly string[] Names =
        {
            "None", "Red", "Blu", "Tie",
        };

        private static readonly ConsoleColor[] Colors =
        {
            ConsoleColor.Gray, ConsoleColor.Red, ConsoleColor.Cyan, ConsoleColor.Green,
        };

        private static readonly ConsoleColor[] BgColors =
        {
            ConsoleColor.Black, ConsoleColor.DarkRed, ConsoleColor.DarkCyan, ConsoleColor.DarkGreen,
        };

        private static readonly int[] Tones =
        {
            100, 500, 400, 200,
        };
    }
}
