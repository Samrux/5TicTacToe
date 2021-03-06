﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;

namespace TTT5
{
    /// <summary>
    /// Wrapper for a 2D array that introduces index looping, and allows access using <see cref="Pos"/> coordinates.
    /// </summary>
    /// <typeparam name="T">The type of the values stored in the board.</typeparam>
    public class Board<T>
    {
        protected readonly T[,] values;


        /// <summary>Number of elements in the board.</summary>
        public int Length => values.Length;

        /// <summary>Number of columns in the board.</summary>
        public int Width => values.GetLength(0);

        /// <summary>Number of rows in the board.</summary>
        public int Height => values.GetLength(1);

        /// <summary>Enumerates through all positions in the board in order.</summary>
        public IEnumerable<Pos> Positions => Enumerable.Range(0, Width * Height).Select(i => new Pos(i % Width, i / Width));



        /// <summary>Creates a new board of the specified constant width and height.</summary>
        public Board(uint width, uint height, T fillValue = default)
        {
            values = new T[width, height];
            if (fillValue != default) Fill(fillValue);
        }

        /// <summary>Creates a new board that is a wrapper for the specified 2D array.</summary>
        public Board(T[,] values)
        {
            this.values = values ?? throw new ArgumentNullException(nameof(values));
        }


        /// <summary>Retrieves an element in the board at the given x and y coordinates,
        /// looping to the other side if the coordinates are outside the board.</summary>
        public T this[int x, int y]
        {
            get => this[(x, y)];
            set => this[(x, y)] = value;
        }

        /// <summary>Retrieves an element in the board at the given position,
        /// looping to the other side if the position is outside the board.</summary>
        public T this[Pos pos]
        {
            get
            {
                pos = Wrap(pos);
                return values[pos.X, pos.Y];
            }

            set
            {
                pos = Wrap(pos);
                values[pos.X, pos.Y] = value;
            }
        }


        public static implicit operator Board<T>(T[,] array) => new Board<T>(array);

        public static explicit operator T[,] (Board<T> board) => board.values;

        public static bool operator ==(Board<T> left, Board<T> right) => left.Equals(right);

        public static bool operator !=(Board<T> left, Board<T> right) => !(left == right);


        /// <summary>Creates a shallow copy of this board.</summary>
        public Board<T> Copy()
        {
            return new Board<T>((T[,])values.Clone());
        }


        /// <summary>Replaces all the elements in the board with the specified value.</summary>
        public Board<T> Fill(T value)
        {
            for (int x = 0; x < Width; x++)
            {
                for (int y = 0; y < Height; y++)
                {
                    values[x, y] = value;
                }
            }

            return this;
        }


        /// <summary>If the given position is outside the board, it will be adjusted to loop around to the other side. 
        /// Positions automatically loop around when accessing an element in the board.</summary>
        public Pos Wrap(Pos pos)
        {
            int x = pos.X, y = pos.Y;
            x %= Width;
            if (pos.X < 0) x += Width;

            y %= Height;
            if (pos.Y < 0) y += Height;

            return (x, y);
        }


        /// <summary>Builds and returns a basic string that represents the elements in the board.</summary>
        public override string ToString()
        {
            var sb = new StringBuilder();
            for (int y = 0; y < Height; y++)
            {
                if (y > 0) sb.Append('\n');
                for (int x = 0; x < Width; x++)
                {
                    sb.Append(values[x, y]);
                    if (x < Width - 1 && typeof(T) != typeof(char)) sb.Append(' ');
                }
            }

            return sb.ToString();
        }
        

        public override bool Equals(object obj) => obj is Board<T> board && values == board.values;

        public override int GetHashCode() => values.GetHashCode();
    }
}
