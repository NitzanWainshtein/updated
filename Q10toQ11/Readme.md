# Q10–Q11: Valgrind & Coverage Runner

This folder provides a single `Makefile` that:
- Runs **Valgrind** analyses:
  - `memcheck` (leaks, invalid reads/writes)
  - `helgrind` (threading/data races) on Q8 (Leader-Follower) and Q9 (Pipeline)
  - `callgrind` (CPU profiling) on Q9 server
- Produces **coverage** reports (gcov/lcov+genhtml) across Q1–Q4, Q6, Q7, Q8, Q9

> Requirement mapping:
> - **(10)** memcheck, helgrind, callgrind – automatic runs with logs in `reports/`
> - **(11)** coverage proof – builds with `--coverage`, executes flows, and collects HTML report

## Usage

```bash
cd Q10toQ11

# Full pipeline: build everything, valgrind (all), coverage + HTML
make

# Or step-by-step:
make build-all
make memcheck
make helgrind
make callgrind
make coverage coverage-html
