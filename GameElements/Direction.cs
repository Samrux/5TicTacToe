
namespace TTT5.GameElements
{
    /// <summary>
    /// The four cardinal directions, useful with <see cref="Pos"/>.
    /// </summary>
    public enum Direction
    {
        None,
        Up,
        Left,
        Down,
        Right,
    }

    public static class DirExtensions
    {
        /// <summary>Converts a direction into a <see cref="Pos"/> vector with the given length.</summary>
        public static Pos ToPos(this Direction dir, int length = 1)
        {
            switch (dir)
            {
                case Direction.Up:    return (0, -length);
                case Direction.Down:  return (0, +length);
                case Direction.Left:  return (-length, 0);
                case Direction.Right: return (+length, 0);
                default:        return Pos.Origin;
            }
        }
    }
}
