using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections.ObjectModel;

namespace Evolution
{
	class Program
	{
		static void Main(string[] args)
		{
			List<Individual> population = new List<Individual>();
			int generationIndex = 1;

			// Ввод эталонной фразы
			Console.Write("Please enter the genome phrase: ");
			string phrase = Console.ReadLine().ToUpper();
			if (!string.IsNullOrEmpty(phrase))
			{
				Individual.Etalon = phrase;
			}

			// Формирование исходной популяции
			for (int i = 0; i < 64; i++)
			{
				population.Add(new Individual());
			}

			// Вывод исходной популяции на экран
			Console.WriteLine("Source population:");
			for (int i = 0; i < population.Count; i++)
			{
				Console.WriteLine("Individual: {0}, Quality: {1}", population[i], population[i].Quality);
			}
			Console.ReadKey();

			// Создание и вывод на экран новых поколений
			while (population.Sum(item => item.Quality) > 12)
			{
				population = Individual.NextGeneration(population);
				generationIndex++;
				Console.WriteLine();
				Console.WriteLine("Next Generation #{0}:", generationIndex);
				population.ForEach(individual => Console.WriteLine("Individual: {0}, Quality: {1}", individual, individual.Quality));
				Console.WriteLine("Average Quality of Generation {0}:", population.Average(individual => individual.Quality));
				Console.ReadKey();
			}
		}
	}

	/// <summary>Особь</summary>
	public class Individual
	{
		#region Конструкторы

			/// <summary>Конструктор по умолчанию</summary>
			public Individual()
			{
				int cycles = Etalon.Length;
				int maxNumber = _symbols.Count;

				for (int i = 0; i < cycles; i++)
				{
					_genotype.Add(_symbols[_generator.Next(maxNumber)]);
				}
			}

			/// <summary>Конструктор порождения от двух других особей</summary>
			/// <param name="father">Папа</param>
			/// <param name="mother">Мама</param>
			public Individual(Individual father, Individual mother)
			{
				List<char> baseGenotype = null;
				List<char> addedGenotype = null;

				// Определение генотипа, на основе которого и будет создаваться новый
				if (father.Genotype.Count > mother.Genotype.Count)
				{
					baseGenotype = new List<char>(father.Genotype);
					addedGenotype = new List<char>(mother.Genotype);
				}
				else
				{
					baseGenotype = new List<char>(mother.Genotype);
					addedGenotype = new List<char>(father.Genotype);
				}

				// Создание нового генотипа на основе базового и добавляемого
				_genotype = baseGenotype;
				for (int i = 0; i < addedGenotype.Count; i++)
				{
					if (_generator.Next(2) == 1)
					{
						_genotype[i] = addedGenotype[i];
					}
				}

				// Внесение случайной мутации
				if (_generator.Next(5) == 4)
				{
					int symbolIndex = _generator.Next(_symbols.Count);
					int genIndex = _generator.Next(_genotype.Count);
					_genotype[genIndex] = _symbols[symbolIndex];
				}
			}

		#endregion

		#region Методы

			/// <summary>Формирование строкового представления особи</summary>
			/// <returns>Строка с генотипом</returns>
			public override string ToString()
			{
				StringBuilder builder = new StringBuilder();
				_genotype.ForEach(gen => builder.Append(gen));

				return builder.ToString();
			}

			/// <summary>Создание нового поколения популяции</summary>
			/// <param name="population">Исходная популяция</param>
			/// <returns>Новое поколение</returns>
			public static List<Individual> NextGeneration(List<Individual> population)
			{
				// Выполнение сортировки
				int index = 0;
				while (index < population.Count - 1)
				{
					if (population[index].Quality > population[index + 1].Quality)
					{
						Individual item = population[index + 1];
						population[index + 1] = population[index];
						population[index] = item;
						index = 0;
					}
					else
					{
						index++;
					}
				}

				// Оставление верхней четверти и получение от ней потомства
				List<Individual> selected = new List<Individual>();
				for (int i = 0; i < population.Count / 4; i++)
				{
					selected.Add(population[i]);
				}

				// Создание нового поколения
				List<Individual> nextGeneration = new List<Individual>();
				for (int i = 0; i < selected.Count - 1; i += 2)
				{
					for (int j = 0; j < population.Count / selected.Count * 2; j++)
					{
						nextGeneration.Add(new Individual(selected[i], selected[i + 1]));
					}
				}

				return nextGeneration;
			}

		#endregion

		#region Операторы

			public static int operator - (Individual first, string etalon)
			{
				int distance = 0;

				List<char> biggerGenotype = null;
				List<char> smallerGenotype = null;

				if (first.Genotype.Count >= etalon.Length)
				{
					biggerGenotype = first.Genotype.ToList();
					smallerGenotype = etalon.ToCharArray().ToList();
				}
				else
				{
					biggerGenotype = etalon.ToCharArray().ToList();
					smallerGenotype = first.Genotype.ToList();
				}

				for (int i = 0; i < biggerGenotype.Count; i++)
				{
					if (i < smallerGenotype.Count)
					{
						distance += Math.Abs(_symbols.IndexOf(biggerGenotype[i]) - _symbols.IndexOf(smallerGenotype[i]));
					}
					else
					{
						distance += _symbols.IndexOf(biggerGenotype[i]);
					}
				}

				return distance;
			}

			public static int operator -(Individual first, Individual second)
			{
				return first - second.ToString();
			}

			public static int operator - (string etalon, Individual second)
			{
				return second - etalon;
			}

		#endregion

		#region Свойства

			/// <summary>Генотип особи</summary>
			public ReadOnlyCollection<char> Genotype { get { return _genotype.AsReadOnly(); } }

			/// <summary>Удалённость особи от эталона</summary>
			public int Quality
			{
				get
				{
					if (_quality < 0)
					{
						_quality = this - Etalon;
					}

					return _quality;
				}
			}

		#endregion

		#region Поля

			/// <summary>Генотип особи</summary>
			private List<char> _genotype = new List<char>();

			/// <summary>Удалённость особи от эталона</summary>
			private int _quality = -1;

		#endregion

		#region Статические поля

			/// <summary>Массив символов - возможных значений генов</summary>
			protected static List<char> _symbols = new List<char>( new char[] { ' ', '!', '-', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'G', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' });

			/// <summary>Эталонная фраза</summary>
			public static string Etalon = "THIS IS A SAMPLE OF EVOLUTION!";

			/// <summary>Генератор случайных чисел</summary>
			protected static Random _generator = new Random(DateTime.Now.Millisecond);

		#endregion
	}
}