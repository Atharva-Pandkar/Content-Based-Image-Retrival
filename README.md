# Content-Based Image Retrieval

A C++ application for content-based image retrieval using OpenCV. Given a target image and an image directory, it finds the most similar images using several matching methods.

**Author:** Atharva Pandkar

---

## Specifications

| Item | Details |
|------|---------|
| **OS** | Windows 11 |
| **IDE** | Visual Studio |
| **Dependencies** | OpenCV |

---

## Required Files

At build/run time, the following should be present:

- **Olympus** (or any directory containing the image database)
- `Driver.cpp`
- `Filter.cpp`, `Filter.h`
- `baseline.cpp`, `baseline.h`
- `histogram.cpp`, `histogram.h`
- `law.cpp`, `law.h`
- `multihistogram.cpp`, `multihistogram.h`
- `textureColor.cpp`, `textureColor.h`

---

## Building & Running

1. Open the project in **Visual Studio** and build.
2. Run **Driver.cpp** (the main executable).

### Command-line usage

```text
<executable> <Target_File> <Dir_name> <Type> <Number_of_results>
```

| Argument | Description |
|----------|-------------|
| **Target_File** | Path to the query/target image |
| **Dir_name** | Directory containing the image database to search |
| **Type** | Matching method (1–5), see below |
| **Number_of_results** | How many similar images to return |

### Matching types (Type)

| # | Method |
|---|--------|
| 1 | Baseline matching (9×9) |
| 2 | Histogram matching |
| 3 | Multi-histogram matching |
| 4 | Texture and color matching |
| 5 | Law filters |

### Example

```text
your_exe.exe path/to/target.jpg path/to/olympus 2 10
```

Searches `path/to/olympus` with histogram matching and returns 10 results.

---

## Example outputs

![Screenshot (2)](https://github.com/Atharva-Pandkar/Content-Based-Image-Retrival/assets/62322017/5b0e9473-c070-480c-8769-b8cc883c58c5)  
![Screenshot (3)](https://github.com/Atharva-Pandkar/Content-Based-Image-Retrival/assets/62322017/ca62ba1e-0052-4885-834b-e94b997978e9)  
![Screenshot (4)](https://github.com/Atharva-Pandkar/Content-Based-Image-Retrival/assets/62322017/2ba9f3ab-5dd3-45ae-906f-39cca1b1cf01)

---

## License

See [LICENSE](LICENSE).
