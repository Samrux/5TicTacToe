using System;

namespace TTT5
{
    /// <summary>
    /// A position in a <see cref="Board{T}"/>, expressed as x and y coordinates.
    /// </summary>
    public readonly struct Pos : IEquatable<Pos>
    {
        public int X { get; }
        public int Y { get; }

        public Pos(int x, int y)
        {
            X = x;
            Y = y;
        }


        public static implicit operator Pos((int x, int y) tuple) => new Pos(tuple.x, tuple.y);


        public override string ToString() => $"({X},{Y})";

        public override int GetHashCode() => X.GetHashCode() ^ Y.GetHashCode();

        public override bool Equals(object obj) => obj is Pos pos && this == pos;

        public bool Equals(Pos pos) => this == pos;


        public static bool operator ==(Pos pos1, Pos pos2) => pos1.X == pos2.X && pos1.Y == pos2.Y;

        public static bool operator !=(Pos pos1, Pos pos2) => !(pos1 == pos2);

        public static Pos operator +(Pos pos1, Pos pos2) => new Pos(pos1.X + pos2.X, pos1.Y + pos2.Y);

        public static Pos operator -(Pos pos1, Pos pos2) => new Pos(pos1.X - pos2.X, pos1.Y - pos2.Y);

        public static Pos operator *(Pos pos, int mult) => new Pos(pos.X * mult, pos.Y * mult);

        public static Pos operator /(Pos pos, int div) => new Pos(pos.X / div, pos.Y / div);
    }
}
